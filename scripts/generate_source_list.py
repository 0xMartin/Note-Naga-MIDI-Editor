import os
import sys
from collections import defaultdict

def collect_files(root_dir):
    header_files = defaultdict(list)
    source_files = defaultdict(list)

    for dirpath, _, filenames in os.walk(root_dir):
        rel_dir = os.path.relpath(dirpath, root_dir)
        for filename in sorted(filenames):
            if filename.endswith(".h") or filename.endswith(".cpp"):
                rel_path = os.path.join(rel_dir, filename) if rel_dir != "." else filename
                full_path = os.path.join(root_dir, rel_path).replace("\\", "/")
                if filename.endswith(".h"):
                    header_files[rel_dir].append(full_path)
                else:
                    source_files[rel_dir].append(full_path)

    return header_files, source_files

def print_section(title, file_dict):
    print(f"set({title}")
    for folder in sorted(file_dict.keys()):
        files = file_dict[folder]
        if not files:
            continue
        folder_comment = f"{folder}" if folder != "." else "src"
        print(f"    # {folder_comment}")
        for file in files:
            print(f"    {file}")
    print(")\n")

def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_source_list.py <path_to_source_directory>")
        sys.exit(1)

    src_dir = sys.argv[1]
    if not os.path.isdir(src_dir):
        print(f"Error: Directory '{src_dir}' does not exist.")
        sys.exit(1)

    headers, sources = collect_files(src_dir)
    print_section("HEADER_FILES", headers)
    print_section("SOURCE_FILES", sources)

if __name__ == "__main__":
    main()