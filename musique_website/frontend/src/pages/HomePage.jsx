import Header from "../components/Header";

export default function HomePage() {
    return (
        <>
            <div className="flex flex-col h-screen">
                <Header></Header>

                <div className="flex-1 flex justify-center items-center">
                    <h1 className="text-3xl font-bold text-blue-950">Hello!</h1>
                </div>
            </div>
        </>
    );
}
