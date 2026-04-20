#!/usr/bin/env python3
"""
Script to reorganize flattened files back into their proper folder structure.
Converts filenames like "resources\\images\\player.png" into resources/images/player.png
"""

import os
import shutil
from pathlib import Path

def reorganize_files(source_dir, dry_run=True):
    """
    Reorganize files with path separators in their names into proper folder structure.
    
    Args:
        source_dir: Directory containing the flattened files
        dry_run: If True, only print what would be done without making changes
    """
    source_path = Path(source_dir)
    
    if not source_path.exists():
        print(f"Error: Directory '{source_dir}' does not exist!")
        return
    
    # Get all files in the source directory (not subdirectories)
    files = [f for f in source_path.iterdir() if f.is_file()]
    
    if not files:
        print("No files found in the directory!")
        return
    
    print(f"Found {len(files)} files to process")
    print(f"Mode: {'DRY RUN (no changes will be made)' if dry_run else 'LIVE (files will be moved)'}")
    print("-" * 60)
    
    moved_count = 0
    skipped_count = 0
    
    for file_path in files:
        filename = file_path.name
        
        # Check if filename contains path separators (backslash or forward slash)
        if '\\' in filename or ('/' in filename and filename.count('/') > 0):
            # Replace backslashes with forward slashes for consistency
            proper_path = filename.replace('\\', '/')
            
            # Create the full destination path
            dest_path = source_path / proper_path
            
            # Create parent directories if they don't exist
            dest_dir = dest_path.parent
            
            if dry_run:
                print(f"Would move:")
                print(f"  From: {filename}")
                print(f"  To:   {proper_path}")
                moved_count += 1
            else:
                try:
                    # Create directories
                    dest_dir.mkdir(parents=True, exist_ok=True)
                    
                    # Move the file
                    shutil.move(str(file_path), str(dest_path))
                    print(f"✓ Moved: {filename} → {proper_path}")
                    moved_count += 1
                except Exception as e:
                    print(f"✗ Error moving {filename}: {e}")
                    skipped_count += 1
        else:
            # File doesn't have path separators, skip it
            skipped_count += 1
    
    print("-" * 60)
    print(f"\nSummary:")
    print(f"  Files to move: {moved_count}")
    print(f"  Files skipped: {skipped_count}")
    
    if dry_run and moved_count > 0:
        print(f"\nThis was a DRY RUN. No files were actually moved.")
        print(f"Run the script with dry_run=False to perform the actual reorganization.")

if __name__ == "__main__":
    import sys
    
    # Get the directory from command line argument or use current directory
    if len(sys.argv) > 1:
        target_dir = sys.argv[1]
    else:
        target_dir = input("Enter the path to the directory with flattened files: ").strip()
        if not target_dir:
            print("Using current directory")
            target_dir = "."
    
    print(f"\nTarget directory: {os.path.abspath(target_dir)}\n")
    
    # First run in dry-run mode to show what will happen
    print("=" * 60)
    print("STEP 1: DRY RUN - Showing what will be done")
    print("=" * 60)
    reorganize_files(target_dir, dry_run=True)
    
    # Ask user if they want to proceed
    print("\n" + "=" * 60)
    response = input("\nDo you want to proceed with moving the files? (yes/no): ").strip().lower()
    
    if response in ['yes', 'y']:
        print("\n" + "=" * 60)
        print("STEP 2: LIVE RUN - Moving files")
        print("=" * 60)
        reorganize_files(target_dir, dry_run=False)
        print("\n✓ Reorganization complete!")
    else:
        print("\nOperation cancelled. No files were moved.")