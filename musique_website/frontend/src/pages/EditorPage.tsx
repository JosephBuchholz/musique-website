import Header from "../components/Header";

export default function EditorPage() {
  return (
    <>
      <div className="flex flex-col h-screen">
        <Header></Header>

        <div className="flex-1 flex justify-center items-center">
          <h1 className="text-3xl font-bold text-blue-950">
            Hello, this is the editor!
          </h1>

          <Canvas></Canvas>
        </div>
      </div>
    </>
  );
}

function Canvas() {
  return (
    <>
      <canvas className="border-2"></canvas>
    </>
  );
}
