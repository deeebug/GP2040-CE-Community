const boardCategories = ['Microcontroller Boards', 'Open Source Devices', 'Closed Source Devices', 'Legacy Devices'];

const boardDefinitions = {
	BentoBox: {
		name: 'ScrubTier BentoBox',
		desc: '',
		category: 'Closed Source Devices',
	},
	DURAL: {
		name: 'BatiKING DURAL',
		desc: '',
		category: 'Legacy Devices',
		url: 'https://github.com/BatiKING/DURAL_DIY',
	},
	FightboardV3: {
		name: 'thnikk Fightboard V3',
		desc: '',
		category: 'Legacy Devices',
	},
	FightboardV3Mirrored: {
		name: 'thnikk Fightboard V3 (Mirrored)',
		desc: '',
		category: 'Legacy Devices',
	},
	FlatboxRev4: {
		name: 'Flatbox Rev 4',
		desc: '',
		category: 'Open Source Devices',
	},
	FlatboxRev5: {
		name: 'Flatbox Rev 5',
		desc: '',
		category: 'Open Source Devices',
	},
	MavercadeKeebfighter: {
		name: 'Mavercade Keebfighter',
		desc: '',
		category: 'Closed Source Devices',
	},
	Pico: {
		name: 'Raspberry Pi Pico',
		desc: 'An affordable dual-core microcontroller developed by Raspberry Pi. This is the standard GP2040-CE build.',
		category: 'Microcontroller Boards',
		url: 'https://www.raspberrypi.com/products/raspberry-pi-pico/',
	},
	PicoAnn: {
		name: 'PicoAnn',
		desc: '',
		category: 'Legacy Devices',
	},
	PicoFightingBoard: {
		name: 'Pico Fighting Board',
		desc: 'Pico breakout board that mimics the style of typical fightstick encoder boards. Developed by FeralAI.',
		category: 'Open Source Devices',
		url: 'https://github.com/FeralAI/PicoFightingBoard',
	},
	RP2040AdvancedBreakoutBoar: {
		name: 'RP2040 Advanced Breakout Board',
		desc: '',
		category: 'Open Source Devices',
	},
	ReflexEncoder: {
		name: 'MiSTerAddons Reflex Arcade Encoder',
		desc: '',
		category: 'Open Source Devices',
	},
	SparkFunProMicro: {
		name: 'SparkFun Pro Micro RP2040',
		desc: 'An RP2040 board in the Pro Micro form factor. The GP2040-CE build is a drop-in replacement for the popular <a href="https://github.com/MickGyver/DaemonBite-Arcade-Encoder">Daemonbite Arcade Encoder</a>.',
		category: 'Microcontroller Boards',
		url: 'https://www.sparkfun.com/products/18288',
	},
	WaveshareZero: {
		name: 'Waveshare RP2040-Zero',
		desc: 'A full RP2040 board in a small form factor. The size, shape and castellated pins make this a great module for board designers that don\'t want to fully integrate an RP2040 chip on their board.',
		category: 'Microcontroller Boards',
		url: 'https://www.waveshare.com/rp2040-zero.htm',
	},
};

const labelData = {
	"GP2040": { "name": "GP2040", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "B1", "B2": "B2", "B3": "B3", "B4": "B4", "L1": "L1", "R1": "R1", "L2": "L2", "R2": "R2", "S1": "S1", "S2": "S2", "L3": "L3", "R3": "R3", "A1": "A1", "A2": "A2" },
	"XInput": { "name": "XInput", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "A", "B2": "B", "B3": "X", "B4": "Y", "L1": "LB", "R1": "RB", "L2": "LT", "R2": "RT", "S1": "Back", "S2": "Start", "L3": "LS", "R3": "RS", "A1": "Guide", "A2": "(A2)" },
	"DirectInput": { "name": "DirectInput", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "2", "B2": "3", "B3": "1", "B4": "4", "L1": "5", "R1": "6", "L2": "7", "R2": "8", "S1": "9", "S2": "10", "L3": "11", "R3": "12", "A1": "13", "A2": "14" },
	"Nintendo Switch": { "name": "Nintendo Switch", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "B", "B2": "A", "B3": "Y", "B4": "X", "L1": "L", "R1": "R", "L2": "ZL", "R2": "ZR", "S1": "Minus", "S2": "Plus", "L3": "LS", "R3": "RS", "A1": "Home", "A2": "Capture" },
	"PS4":    { "name": "PS4", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "Cross", "B2": "Circle", "B3": "Square", "B4": "Triangle", "L1": "L1", "R1": "R1", "L2": "L2", "R2": "R2", "S1": "Share", "S2": "Options", "L3": "L3", "R3": "R3", "A1": "PS", "A2": "Touchpad" },
	"PS3":    { "name": "PS3", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "Cross", "B2": "Circle", "B3": "Square", "B4": "Triangle", "L1": "L1", "R1": "R1", "L2": "L2", "R2": "R2", "S1": "Select", "S2": "Start", "L3": "L3", "R3": "R3", "A1": "PS", "A2": "(A2)" },
	"Arcade": { "name": "Arcade", "Up": "Up", "Down": "Down", "Left": "Left", "Right": "Right", "B1": "K1", "B2": "K2", "B3": "P1", "B4": "P2", "L1": "P4", "R1": "P3", "L2": "K4", "R2": "K3", "S1": "Select", "S2": "Start", "L3": "LS", "R3": "RS", "A1": "Home", "A2": "(A2)" },
};

export {
	boardCategories,
	boardDefinitions,
	labelData,
};
