/**
 * Definition for the song list page.
 */

import Header from "../components/Header";
import { getUser } from "../userauth";
import { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";

/**
 * The song list page. Displays all the songs that
 * the current signed in user has saved to their
 * account.
 *
 * @returns The JSX for this component.
 */
export default function SongListPage() {
    const [user, setUser] = useState(null);
    const [songList, setSongList] = useState([]);

    useEffect(() => {
        getUser().then((user) => {
            setUser(user);
        });

        fetch("/usersongs/getlist")
            .then((response) => response.json())
            .then((data) => {
                console.log(data);
                setSongList(data.songs);
            });
    }, []);

    const songListDisplay = [];

    songList.forEach((song) => {
        songListDisplay.push(
            <SongListItem
                id={song.id}
                title={song.title}
                createdAt={song.created_at}
            ></SongListItem>
        );
    });

    return (
        <>
            <div className="flex flex-col h-screen">
                <Header user={user}></Header>

                <div className="flex flex-col m-4">{songListDisplay}</div>
            </div>
        </>
    );
}

/**
 * An item in the song list. Represents a song.
 *
 * @returns The JSX for this component.
 */
function SongListItem({ id, title, createdAt }) {
    const navigate = useNavigate();

    return (
        <div
            onClick={(e) => {
                navigate(`/editor/${id}/`);
            }}
            className="bg-slate-100 m-1 p-2 border-2 border-slate-100 hover:border-slate-200 hover:cursor-pointer"
        >
            <p className="font-medium text-xl">{title}</p>
            <p className="text-l">{createdAt}</p>
        </div>
    );
}
