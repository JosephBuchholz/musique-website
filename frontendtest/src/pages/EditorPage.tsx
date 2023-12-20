import { useEffect, useRef } from "react";
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

function drawLine(context: CanvasRenderingContext2D, startX: number, startY: number, endX: number, endY: number) {
  context.beginPath();
  context.moveTo(startX, startY);
  context.lineTo(endX, endY);
  context.stroke();
}

function Canvas() {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    let canvas = canvasRef.current;
    if (canvas == null) throw Error("canvas is null");

    let context = canvas.getContext("2d");
    if (context == null) throw Error("canvas is null");

    // @ts-ignore
    var drawRect = () => {
      if (context == null) throw Error("canvas is null");

      context.beginPath();
      context.moveTo(150, 100);
      context.lineTo(70, 30);
      context.stroke();
    };

    drawLine(context, 50, 50, 100, 150);
    //drawRect();
  });

  /*var module: MainModule = {};
  console.log("result: " + module.addNums(1, 2));

  var module: MainModule = {
    onRuntimeInitialized: function() {
      console.log('lerp result: ' + module.addNums(1, 2));
    }
  };*/

  /*var addNums = Module.cwrap(
    "addNums",
    "number",
    ["number", "number"]
  );*/

  return (
    <>
      <canvas ref={canvasRef} className="border-2"></canvas>

      {/*<button onClick={() => {
            console.log(addNums(1, 5))
          }}>Click Me</button>*/}
    </>
  );
}
