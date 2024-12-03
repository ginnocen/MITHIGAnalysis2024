#!/bin/bash
#
# To use:
# bash deleteXrdDir.sh 1

# Define the remote XRootD directory and server
XROOTD_SERVER="root://xrootd5.cmsaf.mit.edu/"
REMOTE_DIR="/store/jdlang/dummydir/"

# Safeguard from accidental run
if [[ $1 -ne 1 ]]; then
  echo "Must add argument: 1"
  exit 0
fi

# Get the list of files and their sizes
FILE_LIST="filesToDelete.txt"
rm $FILE_LIST
xrdfs $XROOTD_SERVER ls -R $REMOTE_DIR >> $FILE_LIST

if [ ! -s $FILE_LIST ]; then
    echo "Could not find remote directory: $REMOTE_DIR"
    exit 1
fi

# Temporary file to store valid lines
temp_file=$(mktemp)

# Process each line and keep only those ending with '.root'
FILE_COUNTER=0
while IFS= read -r line; do
  if [[ "$line" =~ \.root$ ]]; then
    echo "$line" >> "$temp_file"
    FILE_COUNTER=$FILE_COUNTER+1
  fi
done < $FILE_LIST

if [ ! -s "$FILE_LIST" ]; then
    echo "No files found in the remote directory: $REMOTE_DIR"
    exit 1
fi

# Replace the original file with the filtered content
mv "$temp_file" "$FILE_LIST"

echo "You are about to delete $FILE_COUNTER .root files in:"
echo "$XROOTD_SERVER$REMOTE_DIR"
echo "Are you sure? Type 'yes' to confirm:"
read -r CHOICE

if [[ "$CHOICE" == "yes" ]]; then
  # Extract file sizes and calculate the total size
  while read -r LINE; do
      xrdfs $XROOTD_SERVER rm $LINE
  done < $FILE_LIST
  echo "Deletion of all root files from $REMOTE_DIR complete."

else
  echo "Directory deletion was aborted."
fi
