import uproot
import pandas as pd
import numpy as np
import xgboost as xgb
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_auc_score

def process_root_file(input_file, tree_name, output_csv, branches):
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

    # Save the DataFrame to a CSV file
    df.to_csv(output_csv, index=False)
    print(f"Processed data saved to {output_csv}")

    return df

def add_xgboost_score_to_df(X_test, y_test, y_pred, output_csv):
    # Create a new DataFrame from X_test or copy it
    df_scored = X_test.copy()

    # Add the true label
    df_scored["true_label"] = y_test.values

    # Add the XGBoost score (probability of being signal)
    df_scored["xgboost_score"] = y_pred

    # Save the scored DataFrame to CSV
    df_scored.to_csv(output_csv, index=False)
    print(f"Scored DataFrame saved to {output_csv}")

    return df_scored


def train_xgboost(df_signal, df_background, features, test_size=0.3, random_state=42):
    # Label the signal and background
    df_signal["label"] = 1
    df_background["label"] = 0

    # Combine signal and background
    df_combined = pd.concat([df_signal, df_background], axis=0).reset_index(drop=True)

    # Split features (X) and labels (y)
    X = df_combined[features]
    y = df_combined["label"]

    # Create train/test split
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=test_size, random_state=random_state
    )

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

    model.fit(X_train, y_train)

    # Predict probabilities for the positive class (label=1)
    y_pred = model.predict_proba(X_test)[:, 1]

    # Compute the ROC AUC score
    auc_score = roc_auc_score(y_test, y_pred)

    return model, auc_score, X_test, y_test, y_pred

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

    print(f"CSV data from '{csv_file_path}' written to '{root_file_path}' with TTree '{tree_name}'.")

# Usage
input_file = "my_file.root"
tree_name = "Tree"
output_csv = "my_file.csv"
branches = [
    "Dmass",
    "Dchi2cl",
    "Dpt",
    "Dy",
    "Dtrk1Pt",
    "Dtrk2Pt",
    "DsvpvDistance",
    "DsvpvDisErr",
    "DsvpvDistance_2D",
    "DsvpvDisErr_2D",
    "Dalpha",
    "Ddtheta",
    "Dgen",
    "DisSignalCalc",
    "DisSignalCalcPrompt",
    "DisSignalCalcFeeddown"
]

df = process_root_file(input_file, tree_name, output_csv, branches)
df["DsvpvSign"] = df["DsvpvDistance"] / df["DsvpvDisErr"]
df["DsvpvSign_2D"] = df["DsvpvDistance_2D"] / df["DsvpvDisErr_2D"]

df = df[(df["Dpt"] > 1) & (df["Dpt"] < 2)]
# create training samples for signal and background
# select based on Dpt > 2 and Dpt < 4
df_signal = df[df["DisSignalCalcPrompt"] == 1]
df_background = df[df["DisSignalCalcPrompt"] == 0]
features = ["Dchi2cl", "Dalpha", "Ddtheta", "DsvpvSign", "DsvpvSign_2D", "Dtrk1Pt", "Dtrk2Pt"]
# Train the model
model, auc_score, X_test, y_test, y_pred = train_xgboost(df_signal, df_background, features)
print("XGBoost trained successfully.")
print(f"Test ROC AUC: {auc_score:.4f}")

csv_path_scored = "original_scored.csv"
df_scored = apply_xgboost_model(df, model, features, output_csv=csv_path_scored)
csv_to_root(csv_path_scored, "original_scored.root")
# Apply the model to the test data and save the results
