import * as Tone from "tone";

const MIDI_ON_MESSAGE = 0x90;
const MIDI_OFF_MESSAGE = 0x80;

const synth = new Tone.Sampler({
    urls: {
        C4: "C4.mp3",
        "D#4": "Ds4.mp3",
        "F#4": "Fs4.mp3",
        A4: "A4.mp3",
    },
    release: 1,
    baseUrl: "https://tonejs.github.io/audio/salamander/",
}).toDestination();

function getNoteNameFromNumber(noteNumber) {
    var noteName = "C";
    switch (noteNumber) {
        case 0:
            noteName = "C";
            break;
        case 1:
            noteName = "C#";
            break;
        case 2:
            noteName = "D";
            break;
        case 3:
            noteName = "D#";
            break;
        case 4:
            noteName = "E";
            break;
        case 5:
            noteName = "F";
            break;
        case 6:
            noteName = "F#";
            break;
        case 7:
            noteName = "G";
            break;
        case 8:
            noteName = "G#";
            break;
        case 9:
            noteName = "A";
            break;
        case 10:
            noteName = "A#";
            break;
        case 11:
            noteName = "B";
            break;
    }
    return noteName;
}

export default function handleMidiMessage(byteArray) {
    var channel = byteArray[0] & 0x0f;
    var message = byteArray[0] & 0xf0;

    var midiNote = byteArray[1];

    var noteNumber = midiNote % 12;
    var octave = Math.trunc(midiNote / 12) - 1;

    var noteName = getNoteNameFromNumber(noteNumber);

    var note = noteName + octave.toString();

    if (message == MIDI_ON_MESSAGE) synth.triggerAttack(note, Tone.now());
    else if (message == MIDI_OFF_MESSAGE)
        synth.triggerRelease(note, Tone.now());
}
