#!/bin/bash

# Function to create the folder and files
create_folder_and_files() {
    day_number=$(printf "%02d" $1)
    folder_name="day$day_number"
    mkdir "$folder_name"
    touch "$folder_name/$folder_name.cpp" "$folder_name/input.txt" "$folder_name/sample_input.txt" "$folder_name/README.md"
    echo "Created $folder_name with necessary files."
}

# Check if an integer argument is provided
if [[ $1 =~ ^[0-9]+$ ]]; then
    create_folder_and_files $1
else
    # If no integer argument, find the highest dayXX folder and create a new one
    highest_day=$(ls -d day* 2>/dev/null | grep -o '[0-9]\+' | sort -n | tail -n 1)
    if [ -z "$highest_day" ]; then
        create_folder_and_files 1
    else
        next_day=$((highest_day + 1))
        create_folder_and_files $next_day
    fi
fi