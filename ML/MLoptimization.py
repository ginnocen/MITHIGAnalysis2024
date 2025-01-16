import sys
import argparse
import uproot
import pandas as pd
import numpy as np
import xgboost as xgb
from sklearn.model_selection import train_test_split, learning_curve
from sklearn.metrics import roc_auc_score, roc_curve
import yaml
import matplotlib.pyplot as plt
from sklearn.inspection import DecisionBoundaryDisplay


def process_root_file(input_file, tree_name, branches, ptmin, ptmax, ymin, ymax):
    # Load the tree and extract branches
    print(f"Loading tree and extracting branches from : {input_file}")
    with uproot.open(input_file) as file:
        tree = file[tree_name]
        arrays = {branch: tree[branch].array(library="np") for branch in branches}

    # Filter events where len(Dmass) > 0
    dmass = arrays["Dmass"]
    # mask will be used to filter out events with no D candidates
    mask = np.array([len(event) > 0 for event in dmass])

    # IMPORTANT: the operation below is needed cause some of the branches
    # are standard vectors
    filtered_arrays = {branch: np.concatenate(arrays[branch][mask]) for branch in branches}

    # Create a DataFrame from the filtered data
    df = pd.DataFrame(filtered_arrays)
    df = df[(df["Dpt"] > ptmin) & (df["Dpt"] < ptmax) & (df["Dy"] > ymin) & (df["Dy"] < ymax)]
    df["DsvpvSign"] = df["DsvpvDistance"] / df["DsvpvDisErr"]
    df["DsvpvSign_2D"] = df["DsvpvDistance_2D"] / df["DsvpvDisErr_2D"]

    return df


def apply_xgboost_model(df_original, model, features, output_csv=None):

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
    df = pd.read_csv(csv_file_path)
    data_dict = {col: df[col].to_numpy() for col in df.columns}
    with uproot.recreate(root_file_path) as f:
        f[tree_name] = data_dict
    print(
        f"CSV data from '{csv_file_path}' written to '{root_file_path}' with TTree '{tree_name}'.")


def plot_xgb_learning_curve(model,
                            X_train,
                            y_train,
                            cv=5,
                            scoring="roc_auc",
                            n_jobs=-1,
                            train_sizes=np.linspace(0.1, 1.0, 5),
                            ylim=(0.5, 1.4),
                            figsize=(8, 6),
                            title="Learning Curve (XGBoost)"):
    sizes, train_scores, test_scores = learning_curve(estimator=model,
                                                      X=X_train,
                                                      y=y_train,
                                                      cv=cv,
                                                      scoring=scoring,
                                                      n_jobs=n_jobs,
                                                      train_sizes=train_sizes)

    train_scores_mean = np.mean(train_scores, axis=1)
    train_scores_std = np.std(train_scores, axis=1)
    test_scores_mean = np.mean(test_scores, axis=1)
    test_scores_std = np.std(test_scores, axis=1)

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
    plt.savefig("learningcurve.png", dpi=300, bbox_inches="tight")


def plot_xgb_feature_importance(model,
                                X_train,
                                y_train,
                                scoring="roc_auc",
                                n_jobs=-1,
                                figsize=(8, 6),
                                title="Feature Importances (XGBoost)"):
    importances = model.feature_importances_

    # 2. Determine feature names
    if hasattr(X_train, "columns"):
        # If X_train is a DataFrame, use the column names
        feature_names = X_train.columns
    else:
        # Otherwise, create generic names
        feature_names = [f"Feature_{i}" for i in range(X_train.shape[1])]

    # 3. Sort features by importance for a nicer plot
    sorted_idx = np.argsort(importances)
    sorted_importances = importances[sorted_idx]
    sorted_features = [feature_names[i] for i in sorted_idx]

    # 4. Plot a horizontal bar chart
    plt.figure(figsize=figsize)
    plt.barh(sorted_features, sorted_importances, color="skyblue")
    plt.title(title)
    plt.xlabel("Importance")
    plt.ylabel("Features")
    plt.grid(True, axis="x", alpha=0.5)
    plt.tight_layout()
    plt.savefig("feature_importance..png", dpi=300, bbox_inches="tight")

    return importances


branches = [
    "Dmass", "Dchi2cl", "Dpt", "Dy", "Dtrk1Pt", "Dtrk2Pt", "DsvpvDistance", "DsvpvDisErr",
    "DsvpvDistance_2D", "DsvpvDisErr_2D", "Dalpha", "Ddtheta", "Dgen", "DisSignalCalc",
    "DisSignalCalcPrompt", "DisSignalCalcFeeddown", "DpassCut23LowPt"
]

parser = argparse.ArgumentParser(description="Process arguments for MLoptimization.py")
parser.add_argument("--random_state", default=42)
parser.add_argument(
    "--input_file_mc",
    default=
    "/Users/ginnocen/Desktop/MITHIGAnalysis2024/Skims/SkimsMC/20241216_v1_filelist20241216_Pthat2_ForceD0Decay100M_BeamA_v1/mergedfile.root"
)
parser.add_argument("--tree_name", default="Tree")
parser.add_argument("--ptmin", default=1)
parser.add_argument("--ptmax", default=2)
parser.add_argument("--ymin", default=-1)
parser.add_argument("--ymax", default=+1)
parser.add_argument("--output_model", default="XGBoostMConly_pt_p1p2_y_m1p1.json")
parser.add_argument("--output_csv", default="outputmctraining_pt_p1p2_y_m1p1.cvs")
parser.add_argument("--output_root", default="outputmctraining_pt_p1p2_y_m1p1.root")
args = parser.parse_args()

random_state = args.random_state
input_file_mc = args.input_file_mc
tree_name = args.tree_name
ptmin = float(args.ptmin)
ptmax = float(args.ptmax)
ymin = float(args.ymin)
ymax = float(args.ymax)
output_model = args.output_model
output_csv = args.output_csv
output_root = args.output_root
# Here we define the signifiance variables, which are not defined in the skimmed trees

df = process_root_file(input_file_mc, tree_name, branches, ptmin, ptmax, ymin, ymax)

# here we consider only D candidates within a range of pT and rapidity (boundaries are
# read from the config file

# create training samples for signal and background
# in this version of the code, the signal is defined as the prompt D candidates,
# all the rest is tagged as background

df_signal = df[df["DisSignalCalcPrompt"] == 1]
df_background = df[df["DisSignalCalcPrompt"] == 0]

# for convenience, we label the candidates in the signal df as label = 1
# and those in the background df as label = 0

df_signal["label"] = 1
df_background["label"] = 0

df_combined = pd.concat([df_signal, df_background], axis=0).reset_index(drop=True)

# features are the variables used for the optimization
features = ["Dchi2cl", "Dalpha", "DsvpvSign", "Dtrk1Pt", "Dtrk2Pt"]

# Split features (X) and labels (y)
X = df_combined[features]
y = df_combined["label"]

# Create train/test split, test_size defines the fraction of candidates that are used for testing
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
plot_xgb_learning_curve(model=model,
                        X_train=X_train,
                        y_train=y_train,
                        cv=5,
                        scoring="roc_auc",
                        train_sizes=np.linspace(0.1, 1.0, 5),
                        ylim=(0.5, 1.4))

# Fit the model to the full training set
# FIXME: we should make sure that the model is re-trained from scratch

model.fit(X_train, y_train)
model.save_model(output_model)

plot_xgb_feature_importance(
    model=model,
    X_train=X,  # Only used to extract feature names
    y_train=y,  # Not used here
    scoring="roc_auc",
    n_jobs=-1,
    figsize=(6, 4),
    title="Feature Importances (Fully Trained Model)")

# Predict probabilities for the positive class (label=1)
y_pred_test = model.predict_proba(X_test)[:, 1]

# Compute the ROC AUC score
auc_score_test = roc_auc_score(y_test, y_pred_test)
print(f"Test ROC AUC: {auc_score_test:.4f}")

fpr, tpr, thresholds = roc_curve(y_test, y_pred_test)
plt.figure()
plt.plot(fpr, tpr, label=f"ROC Curve (AUC = {auc_score_test:.2f})")
plt.plot([0, 1], [0, 1], 'k--', label="Random Guess")
plt.xlabel("False Positive Rate")
plt.ylabel("True Positive Rate")
plt.title("ROC Curve")
plt.legend(loc="lower right")
plt.savefig("roccurve.png", dpi=300, bbox_inches="tight")
