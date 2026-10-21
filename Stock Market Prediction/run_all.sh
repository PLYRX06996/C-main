#!/usr/bin/env bash
# ==============================================================================
# Automated Sequential Pipeline Runner for TAI_project
#
# Usage:
#   ./run_all.sh           # Runs all stage scripts in sequential order
#   ./run_all.sh stage1    # Runs only Stage 1 scripts
#   ./run_all.sh stage2    # Runs only Stage 2 scripts
#   ./run_all.sh stage3    # Runs only Stage 3 scripts
#   ./run_all.sh stage4    # Runs only Stage 4 scripts
# ==============================================================================

# Ensure execution from project root
cd "$(dirname "$0")"

# Explicitly activate local virtual environment
if [ -f ".venv/bin/activate" ]; then
    source .venv/bin/activate
fi

# Always prioritize local virtual environment Python
if [ -f ".venv/bin/python" ]; then
    PYTHON_BIN=".venv/bin/python"
else
    PYTHON_BIN="$(which python)"
fi
echo "Using Python: $PYTHON_BIN"

FILTER="${1:-all}"

# Find scripts based on filter
if [ "$FILTER" = "all" ]; then
    SCRIPTS=(src/stage*.py)
else
    SCRIPTS=(src/${FILTER}*.py)
fi

if [ ${#SCRIPTS[@]} -eq 0 ] || [ ! -e "${SCRIPTS[0]}" ]; then
    echo "No scripts found matching pattern: src/${FILTER}*.py"
    exit 1
fi

TOTAL=${#SCRIPTS[@]}
CURRENT=1
START_ALL=$(date +%s)

echo "=================================================================="
echo " Starting Sequential Execution of $TOTAL Script(s)"
echo " Filter: $FILTER"
echo "=================================================================="

for script in "${SCRIPTS[@]}"; do
    echo ""
    echo "------------------------------------------------------------------"
    echo " [$CURRENT/$TOTAL] Running: $script"
    echo " Started at: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "------------------------------------------------------------------"
    
    START_SCRIPT=$(date +%s)
    
    if $PYTHON_BIN "$script"; then
        END_SCRIPT=$(date +%s)
        ELAPSED=$((END_SCRIPT - START_SCRIPT))
        echo "--> SUCCESS: $script completed in ${ELAPSED}s"
    else
        echo "--> FAILED: $script encountered an error (exit code $?)"
        read -p "Do you want to continue with next scripts? (y/n) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "Pipeline aborted."
            exit 1
        fi
    fi
    
    CURRENT=$((CURRENT + 1))
done

END_ALL=$(date +%s)
TOTAL_ELAPSED=$((END_ALL - START_ALL))

echo ""
echo "=================================================================="
echo " All selected scripts finished in ${TOTAL_ELAPSED}s"
echo "=================================================================="
