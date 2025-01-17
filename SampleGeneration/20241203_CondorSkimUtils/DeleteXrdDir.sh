#!/bin/bash
# DeleteXrdDir.sh
# xrootd cannot delete a directory unless it is empty

# Default values (will be used if no args given)
XROOTD_SERVER_DEF="root://xrootd.cmsaf.mit.edu"
REMOTE_DIR_DEF="/store/user/jdlang/run3_2023Data_Jan2024ReReco_Skims_20250115/"

# Overwrite if run with args
XROOTD_SERVER=${1:-$XROOTD_SERVER_DEF}
REMOTE_DIR=${2:-$REMOTE_DIR_DEF}

# Make list of all files in the directory
TEMP_LIST=$(mktemp)
xrdfs $XROOTD_SERVER ls -R $REMOTE_DIR >> $TEMP_LIST
if [ -z "$TEMP_LIST" ]; then
    echo "Unable to list remote directory:"
    echo $XROOTD_SERVER$REMOTE_DIR
    exit 0
fi

DATE=$(date +%Y%m%d)
DEL_FILE_LIST="delete_files_${DATE}.txt"
DEL_DIR_LIST="delete_directories_${DATE}.txt"
rm -f $DEL_FILE_LIST
rm -f $DEL_DIR_LIST

# Create lists of files and folders within given dir
FILE_COUNTER=0
DIR_COUNTER=0
while IFS= read -r LINE; do
  if [[ $(basename "$LINE") == *.* ]]; then
    echo "$LINE" >> "$DEL_FILE_LIST"
    ((FILE_COUNTER++))
  else
    echo "$LINE" >> "$DEL_DIR_LIST"
    ((DIR_COUNTER++))
  fi
done < $TEMP_LIST
rm $TEMP_LIST

# Exit if nothing to delete
if [[ $FILE_COUNTER -eq 0 && $DIR_COUNTER -eq 0 ]]; then
    echo "Found no files or subfolders in remote directory:"
    echo $XROOTD_SERVER$REMOTE_DIR
    exit 0
fi

echo "
You are about to delete $FILE_COUNTER files and $DIR_COUNTER subdirectories 
in the following directory:
$XROOTD_SERVER$REMOTE_DIR
"
read -p "Are you sure? (type 'yes' to confirm):
" -r
if [[ "$REPLY" == "yes" ]]; then
  # First, delete the files
  while IFS= read -r LINE; do
    xrdfs $XROOTD_SERVER rm $LINE
  done < $DEL_FILE_LIST
  # Then, delete the directories
  while IFS= read -r LINE; do
    xrdfs $XROOTD_SERVER rmdir $LINE
  done < $DEL_DIR_LIST
  # Finally delete the original provided directory
  xrdfs $XROOTD_SERVER rmdir $REMOTE_DIR
  # Cleanup only if deleted
  rm -f $DEL_FILE_LIST
  rm -f $DEL_DIR_LIST
else
  echo "Response was not 'yes', so files will NOT be deleted."
  echo "Lists of files and folders that would be deleted are here:"
  echo "$DEL_FILE_LIST"
  echo "$DEL_DIR_LIST"
fi
