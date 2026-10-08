import os
import shutil
import re

def strip_comments_and_formatting(code):
    # Remove single line python comments
    code = re.sub(r'#.*', '', code)
    # Remove print statements containing === or ---
    code = re.sub(r'[ \t]*print\(.*[\'\"][ \t]*[-=]{3,}[ \t]*[\'\"].*\).*?\n', '', code)
    # Clean up double blank lines caused by removing comments
    code = re.sub(r'\n\s*\n', '\n\n', code)
    return code.strip() + '\n'

def main():
    source_dir = os.path.dirname(os.path.abspath(__file__))
    target_dir = os.path.normpath(os.path.join(source_dir, '..', 'Stock_Market_Prediction_Final'))

    # Directories we want to copy over to the public repo
    dirs_to_sync = ['src']

    print(f"Exporting cleaned files to: {target_dir}")
    os.makedirs(target_dir, exist_ok=True)

    # 1. Handle SRC directory (scrubbing Python code)
    for d in dirs_to_sync:
        src_path = os.path.join(source_dir, d)
        if not os.path.exists(src_path):
            continue

        for root, _, files in os.walk(src_path):
            for file in files:
                if not file.endswith('.py'):
                    continue
                
                rel_path = os.path.relpath(os.path.join(root, file), source_dir)
                dest_path = os.path.join(target_dir, rel_path)
                os.makedirs(os.path.dirname(dest_path), exist_ok=True)

                with open(os.path.join(root, file), 'r') as f:
                    content = f.read()

                clean_content = strip_comments_and_formatting(content)

                with open(dest_path, 'w') as f:
                    f.write(clean_content)

    # 2. Handle MODELS directory (Copying binary files directly without text processing)
    models_src = os.path.join(source_dir, 'models')
    models_dest = os.path.join(target_dir, 'models')
    if os.path.exists(models_src):
        if os.path.exists(models_dest):
            shutil.rmtree(models_dest) # Wipe old models cleanly
        shutil.copytree(models_src, models_dest)
        print("Copied models/ directory.")

    # 3. Create requirements.txt
    req_path = os.path.join(target_dir, 'requirements.txt')
    with open(req_path, 'w') as f:
        f.write("pandas\nnumpy\nscikit-learn\nxgboost\nlightgbm\noptuna\njoblib\n")
    print("Generated requirements.txt")

    # 4. Create .gitignore (blocking datasets and environments)
    gitignore_path = os.path.join(target_dir, '.gitignore')
    with open(gitignore_path, 'w') as f:
        f.write("# Environments\n.venv/\nenv/\n\n# Datasets (Too large for GitHub)\ntrain/\ntest/\n*.csv\n\n# IDEs\n.vscode/\n.idea/\n")
    print("Generated .gitignore file.")

    print("\nExport complete! The folder is ready for GitHub.")

if __name__ == '__main__':
    main()
