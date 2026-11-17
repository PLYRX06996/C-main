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
#   ./run_all.sh stage5    # Runs only Stage 5 scripts
# ==============================================================================

# Ensure execution from project root
cd "$(dirname "$0")"

# Activate virtual environment if present
if [ -f ".venv/bin/activate" ]; then
    source .venv/bin/activate
elif [ -f "venv/bin/activate" ]; then
    source venv/bin/activate
elif [ -f "env/bin/activate" ]; then
    source env/bin/activate
fi

# Prioritize virtual environment Python, fallback to system python3 or python
if [ -x ".venv/bin/python" ]; then
    PYTHON_BIN=".venv/bin/python"
elif [ -x "venv/bin/python" ]; then
    PYTHON_BIN="venv/bin/python"
elif [ -x "env/bin/python" ]; then
    PYTHON_BIN="env/bin/python"
elif command -v python3 >/dev/null 2>&1; then
    PYTHON_BIN="$(command -v python3)"
else
    PYTHON_BIN="$(command -v python)"
fi
echo "Using Python: $PYTHON_BIN"
export PYTHONUNBUFFERED=1

if [ "$#" -eq 0 ] || [ "$1" = "all" ]; then
    SCRIPTS=(src/stage*.py src/eval_rigorous_test_benchmark.py src/verify_data_integrity.py)
    FILTER="all"
else
    SCRIPTS=()
    FILTER="$*"
    for arg in "$@"; do
        if [ -f "$arg" ]; then
            SCRIPTS+=("$arg")
        elif [ -f "src/$arg" ]; then
            SCRIPTS+=("src/$arg")
        elif [ -f "src/${arg}.py" ]; then
            SCRIPTS+=("src/${arg}.py")
        else
            matches=(src/${arg}*.py)
            if [ -e "${matches[0]}" ]; then
                SCRIPTS+=("${matches[@]}")
            fi
        fi
    done
fi

if [ ${#SCRIPTS[@]} -eq 0 ] || [ ! -e "${SCRIPTS[0]}" ]; then
    echo "No scripts found matching: $FILTER"
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
