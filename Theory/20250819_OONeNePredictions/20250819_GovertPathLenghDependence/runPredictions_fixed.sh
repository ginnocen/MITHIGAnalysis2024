#!/usr/bin/env bash
set -euo pipefail

# Always resolve the macro next to this script
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)"
# Change this to MakeRAAGraph_fixed.C if that's your filename
MACRO="${SCRIPT_DIR}/MakeRAAGraph.C"

run_one() {
  local csv="$1" label="$2"
  [[ -f "$csv" ]] || { echo "Missing: $csv" >&2; return; }
  echo "-> Drawing ${label}.png"
  # Load (compile if needed) and call DrawRAA in one go; no auto-exec
  root -l -b -q -e "gROOT->ProcessLine(\".L ${MACRO}+\"); DrawRAA(\"$csv\",\"$label\")"
}

# Examples:
 run_one "NeNe-MeanRAA_vs_pT_at_0–100%_Centrality.csv" "Neon-Neon"
 run_one "OO-MeanRAA_vs_pT_at_0–100%_Centrality.csv"   "Oxygen-Oxygen"
echo "All done."

