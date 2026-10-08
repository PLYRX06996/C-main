import os
import shutil
import re

def strip_comments_and_formatting(code):
    # Remove single line python comments
    code = re.sub(r'#.*', '', code)

    # Remove print statements containing === or ---
    code = re.sub(r'[ \t]*print\(.*[\'\"][ \t]*[-=]{3,}[ \t]*[\'\"].*\).*?\n', '', code)

    # Remove strings that are just "--- Training xxx ---" left over in normal prints
    # Note: the one above catches print("--- Training ---"). This is just an extra safety.

    # Clean up double blank lines caused by removing comments
    code = re.sub(r'\n\s*\n', '\n\n', code)

    return code.strip() + '\n'

def main():
    source_dir = os.path.dirname(os.path.abspath(__file__))
    # Create the clean target folder one directory up
    target_dir = os.path.normpath(os.path.join(source_dir, '..', 'Stock_Market_Prediction_App'))

    # Directories we want to copy over to the public repo
    dirs_to_sync = ['src']

    print(f"Exporting cleaned files to: {target_dir}")

    for d in dirs_to_sync:
        src_path = os.path.join(source_dir, d)
        if not os.path.exists(src_path):
            continue

        for root, _, files in os.walk(src_path):
            for file in files:
                if not file.endswith('.py'):
                    continue

                # Calculate paths
                rel_path = os.path.relpath(os.path.join(root, file), source_dir)
                dest_path = os.path.join(target_dir, 'TAI_project_clean', rel_path)

                # Make sure the target folder exists
                os.makedirs(os.path.dirname(dest_path), exist_ok=True)

                # Read original AI code
                with open(os.path.join(root, file), 'r') as f:
                    content = f.read()

                # Strip out the AI tracks (comments and fancy lines)
                clean_content = strip_comments_and_formatting(content)

                # Overwrite (this handles modifications and new files perfectly)
                with open(dest_path, 'w') as f:
                    f.write(clean_content)

    print("Export complete! You can now safely push the target directory to GitHub.")

if __name__ == '__main__':
    main()
