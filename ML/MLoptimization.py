import uproot
import pandas as pd
import numpy as np
import xgboost as xgb
from sklearn.model_selection import train_test_split, learning_curve
from sklearn.metrics import roc_auc_score
import yaml
import matplotlib.pyplot as plt


def process_root_file(input_file, tree_name, branches):
    # Load the tree and extract branches
    print(f"Processing file: {input_file}")
    with uproot.open(input_file) as file:
        tree = file[tree_name]
        arrays = {branch: tree[branch].array(library="np") for branch in branches}

    # Filter events where len(Dmass) > 0
    dmass = arrays["Dmass"]
    mask = np.array([len(event) > 0 for event in dmass])
    filtered_arrays = {branch: np.concatenate(arrays[branch][mask]) for branch in branches}

    # Create a DataFrame from the filtered data
    df = pd.DataFrame(filtered_arrays)

    return df



def apply_xgboost_model(df_original, model, features, output_csv=None):
    import numpy as np

    # Check that df_original contains all the required features
    for f in features:
        if f not in df_original.columns:
            raise ValueError(f"Feature '{f}' not found in df_original.")

    # Predict probabilities using the trained XGBoost model
    # [:, 1] gives the probability of the positive class (signal)
    scores = model.predict_proba(df_original[features])[:, 1]

    # Create a copy of df_original so we don't modify it in-place
    df_scored = df_original.copy()

    # Add the predicted XGBoost score as a new column
    df_scored["xgboost_score"] = scores

    # Optionally save to CSV
    if output_csv is not None:
        df_scored.to_csv(output_csv, index=False)
        print(f"Scored DataFrame saved to {output_csv}")

    return df_scored


def csv_to_root(csv_file_path, root_file_path, tree_name="Tree"):
    # 1. Read the CSV file into a DataFrame
    df = pd.read_csv(csv_file_path)

    # 2. Convert the DataFrame to a dictionary of column_name -> numpy array
    #    uproot can write these columns as a TTree
    data_dict = {col: df[col].to_numpy() for col in df.columns}

    # 3. Create or overwrite a ROOT file and write the TTree
    with uproot.recreate(root_file_path) as f:
        f[tree_name] = data_dict

    print(
        f"CSV data from '{csv_file_path}' written to '{root_file_path}' with TTree '{tree_name}'.")


def plot_xgb_learning_curve(model, X_train, y_train,
                            cv=5,
                            scoring="roc_auc",
                            n_jobs=-1,
                            train_sizes=np.linspace(0.1, 1.0, 5),
                            ylim=(0.5, 1.4),
                            figsize=(8, 6),
                            title="Learning Curve (XGBoost)"):
    sizes, train_scores, test_scores = learning_curve(
        estimator=model,
        X=X_train,
        y=y_train,
        cv=cv,
        scoring=scoring,
        n_jobs=n_jobs,
        train_sizes=train_sizes
    )

    train_scores_mean = np.mean(train_scores, axis=1)
    train_scores_std  = np.std(train_scores, axis=1)
    test_scores_mean  = np.mean(test_scores, axis=1)
    test_scores_std   = np.std(test_scores, axis=1)

    plt.figure(figsize=figsize)
    plt.plot(sizes, train_scores_mean, 'o-', color='r', label='Training score')
    plt.fill_between(sizes,
                     train_scores_mean - train_scores_std,
                     train_scores_mean + train_scores_std,
                     alpha=0.2,
                     color='r')

    plt.plot(sizes, test_scores_mean, 'o-', color='g', label='Cross-validation score')
    plt.fill_between(sizes,
                     test_scores_mean - test_scores_std,
                     test_scores_mean + test_scores_std,
                     alpha=0.2,
                     color='g')

    plt.title(title)
    if ylim is not None:
        plt.ylim(*ylim)
    plt.xlabel('Training Examples')
    plt.ylabel('ROC AUC' if scoring == "roc_auc" else scoring)
    plt.legend(loc='best')
    plt.grid(True)
    plt.show()

with open("config.yaml", "r") as f:
    config = yaml.safe_load(f)

# 2. Extract necessary info from the config
input_file_mc = config["root_file_mc"]
tree_name = config["tree_name"]
output_csv = config["output_csv"]
output_root = config["output_root"]
ptmin = config["ptmin"]
ptmax = config["ptmax"]
ymin = config["ymin"]
ymax = config["ymax"]
output_model = config["output_model"]

branches = [
    "Dmass", "Dchi2cl", "Dpt", "Dy", "Dtrk1Pt", "Dtrk2Pt", "DsvpvDistance", "DsvpvDisErr",
    "DsvpvDistance_2D", "DsvpvDisErr_2D", "Dalpha", "Ddtheta", "Dgen", "DisSignalCalc",
    "DisSignalCalcPrompt", "DisSignalCalcFeeddown"
]

df = process_root_file(input_file_mc, tree_name, branches)
df["DsvpvSign"] = df["DsvpvDistance"] / df["DsvpvDisErr"]
df["DsvpvSign_2D"] = df["DsvpvDistance_2D"] / df["DsvpvDisErr_2D"]

df = df[(df["Dpt"] > ptmin) & (df["Dpt"] < ptmax) & (df["Dy"] > ymin) & (df["Dy"] < ymax)]
# create training samples for signal and background
# select based on Dpt > 2 and Dpt < 4
df_signal = df[df["DisSignalCalcPrompt"] == 1]
df_background = df[df["DisSignalCalcPrompt"] == 0]
df_signal["label"] = 1
df_background["label"] = 0
features = ["Dchi2cl", "Dalpha", "DsvpvSign", "Dtrk1Pt", "Dtrk2Pt"]
df_combined = pd.concat([df_signal, df_background], axis=0).reset_index(drop=True)

# Split features (X) and labels (y)
X = df_combined[features]
y = df_combined["label"]

# Create train/test split
random_state = 42
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=random_state)

# Initialize and train the XGBoost classifier
model = xgb.XGBClassifier(
    max_depth=3,
    learning_rate=0.1,
    n_estimators=850,
    objective="binary:logistic",
    n_jobs=10,
    gamma=0.0,
    min_child_weight=3,
    subsample=0.8,
    colsample_bytree=0.8,
    colsample_bynode=1,
    random_state=0,
    tree_method="hist",
)


# Plot the learning curve
plot_xgb_learning_curve(
    model=model,
    X_train=X_train,
    y_train=y_train,
    cv=5,
    scoring="roc_auc",
    train_sizes=np.linspace(0.1, 1.0, 5),
    ylim=(0.5, 1.4)
)

# Fit the model to the full training set
model.fit(X_train, y_train)

# Predict probabilities for the positive class (label=1)
y_pred_test = model.predict_proba(X_test)[:, 1]

# Compute the ROC AUC score
auc_score_test = roc_auc_score(y_test, y_pred_test)
print(f"Test ROC AUC: {auc_score_test:.4f}")

# Add the XGBoost score to original DataFrame and save it to a CSV file
df_scored = apply_xgboost_model(df, model, features, output_csv=output_csv)
csv_to_root(output_csv, output_root, tree_name="Tree")
model.save_model(output_model)
# Apply the model to the test data and save the results
