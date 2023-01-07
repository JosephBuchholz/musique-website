import json

filepath = input("File path to save data to: ")
title = input("Enter title of song: ")
artists = input("Enter artists of song (make sure not to put spaces between commas): ")
album = input("Enter album of the song: ")
release_date = int(input("Enter release date of the song: "))
instruments = input("Enter instruments of song: ")
tags = input("Enter tags of song: ")
difficulty = float(input("Enter difficulty of song: "))
desc = input("Enter desctiption for the song: ")
sources = input("Enter video sources for the song: ")

confirm = input("Jason or Text (JSON or TXT or NO)?: ")

if confirm == "NO":
    print("Exiting")
    exit()
elif confirm == "J":
    returnDict = {
        "title": title,
        "artists": artists,
        "album": album,
        "release_date": release_date,
        "instruments": instruments,
        "tags": tags,
        "difficulty": difficulty,
        "description": desc,
        "video_sources": sources
    }

    returnJson = json.dumps(returnDict)

    f = open(filepath, "w")
    f.write(returnJson)
    f.close()
elif confirm == "TXT":
    f = open(filepath, "w")

    data = ""
    data += "title: " + title + "\n"
    data += "artists: " + artists + "\n"
    data += "album: " + album + "\n"
    data += "release_date: " + str(release_date) + "\n"
    data += "instruments: " + instruments + "\n"
    data += "tags: " + tags + "\n"
    data += "difficulty: " + str(difficulty) + "\n"
    data += "desc: " + desc + "\n"
    data += "sources: " + sources + "\n"

    f.write(data)
    f.close()

print("Finnished")