#!/usr/bin/bash

LOG_FOLDER="$HOME/logs"
LOG_FILE="$LOG_FOLDER/log"

PROJECT_FOLDER="$HOME/labs/lab1/task1"
ARCHIVE="$LOG_FOLDER/project_archive.tar.gz"

datetime=$(date '+%d-%m-%Y | %H:%M:%S')
linux_kernel=$(uname -r)

#1
if ! [ -d "$LOG_FOLDER" ]; then
    mkdir $LOG_FOLDER
    echo "Було створено каталог $LOG_FOLDER "
fi

touch $LOG_FILE
echo -e "\n$datetime | $USER | $HOSTNAME | $linux_kernel" >> $LOG_FILE

#2
make clean -C "$PROJECT_FOLDER"
tar -czvf "$ARCHIVE" "$PROJECT_FOLDER"

archive_size=$(du -sh "$ARCHIVE" | cut -f 1)
smallest_file=$(du $PROJECT_FOLDER -ab | sort -rh | tail -n 1 | cut -f 2 )
largest_file=$(du $PROJECT_FOLDER -ab | sort -rh | head -n 2 | tail -n 1 | cut -f 2)

echo -e \
"\tРозмір архіву: $archive_size; 
\tНайменший файл: $smallest_file;
\tНайбільший файл: $largest_file" >> $LOG_FILE

# 3
chmod -R g+rwX "$HOME"

# 4
echo "Вміст теки $HOME:"
select item in $(ls -a "$HOME"); do
    if [ -f "$HOME/$item" ]; then
        cat "$HOME/$item"
        break
    elif [ -d "$HOME/$item" ]; then
        ls -a "$HOME/$item"
        break
    fi
done

# 5
chmod +x "$0"