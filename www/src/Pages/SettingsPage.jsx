import React, { useContext, useEffect, useState } from 'react';
import { AppContext } from '../Contexts/AppContext';
import { Button, Form, Modal } from 'react-bootstrap';
import { Formik, useFormikContext } from 'formik';
import { NavLink } from "react-router-dom";
import * as yup from 'yup';

import Section from '../Components/Section';
import WebApi from '../Services/WebApi';
import { BUTTONS, BUTTON_MASKS } from '../Data/Buttons';

const PS4Mode = 4;
const INPUT_MODES = [
	{ label: 'XInput', value: 0 },
	{ label: 'Nintendo Switch', value: 1 },
	{ label: 'PS3/DirectInput', value: 2 },
	{ label: 'Keyboard', value: 3 },
	{ label: 'PS4', value: PS4Mode }
];

const DPAD_MODES = [
	{ label: 'D-pad', value: 0 },
	{ label: 'Left Analog', value: 1 },
	{ label: 'Right Analog', value: 2 },
];

const SOCD_MODES = [
	{ label: 'Up Priority', value: 0 },
	{ label: 'Neutral', value: 1 },
	{ label: 'Last Win', value: 2 },
	{ label: 'First Win', value: 3 },
	{ label: 'Off', value: 4 },
];

const HOTKEY_MASKS = [
	{ label: 'Up', value: 1<<0 },
	{ label: 'Down', value: 1<<1 },
	{ label: 'Left', value: 1<<2 },
	{ label: 'Right', value: 1<<3 },
];

const HOTKEY_ACTIONS = [
	{ label: 'No Action', value: 0 },
	{ label: 'Dpad Digital', value: 1 },
	{ label: 'Dpad Left Analog', value: 2 },
	{ label: 'Dpad Right Analog', value: 3 },
	{ label: 'Home Button', value: 4 },
	{ label: 'Capture Button', value: 5 },
	{ label: 'SOCD UP Priority', value: 6 },
	{ label: 'SOCD Neutral', value: 7 },
	{ label: 'SOCD Last Win', value: 8 },
	{ label: 'SOCD First Win', value: 11 },
	{ label: 'SOCD Cleaning Off', value: 12 },
	{ label: 'Invert X Axis', value: 9 },
	{ label: 'Invert Y Axis', value: 10 },
];

const FORCED_SETUP_MODES = [
	{ label: 'Off', value: 0 },
	{ label: 'Disable Input-Mode Switch', value: 1 },
	{ label: 'Disable Web-Config', value: 2 },
	{ label: 'Disable Input-Mode Switch and Web-Config', value: 3 },
];

const hotkeySchema = {
	action: yup.number().required().oneOf(HOTKEY_ACTIONS.map(o => o.value)).label('Hotkey Action'),
	buttonsMask: yup.number().required().label('Button Mask'),
	auxMask: yup.number().required().label('Function Key')
};

const hotkeyFields = Array(12).fill(0).reduce((acc, a, i) => {
	const number = String(i + 1).padStart(2, '0');
	const newSchema = yup.object().label('Hotkey ' + number).shape({ ...hotkeySchema });
	acc["hotkey" + number] = newSchema;
	return acc;
}, {});

const schema = yup.object().shape({
	dpadMode : yup.number().required().oneOf(DPAD_MODES.map(o => o.value)).label('D-Pad Mode'),
	...hotkeyFields,
	inputMode: yup.number().required().oneOf(INPUT_MODES.map(o => o.value)).label('Input Mode'),
	socdMode : yup.number().required().oneOf(SOCD_MODES.map(o => o.value)).label('SOCD Cleaning Mode'),
	switchTpShareForDs4: yup.number().required().label('Switch Touchpad and Share'),
	forcedSetupMode : yup.number().required().oneOf(FORCED_SETUP_MODES.map(o => o.value)).label('SOCD Cleaning Mode'),
	lockHotkeys: yup.number().required().label('Lock Hotkeys'),
});

const FormContext = ({ setButtonLabels }) => {
	const { values, setValues } = useFormikContext();

	useEffect(() => {
		async function fetchData() {
			const options = await WebApi.getGamepadOptions()
			setValues(options);
			setButtonLabels({ swapTpShareLabels: (options.switchTpShareForDs4 === 1) && (options.inputMode === 4) });
		}
		fetchData();
	}, [setValues]);

	useEffect(() => {
		if (!!values.dpadMode)
			values.dpadMode = parseInt(values.dpadMode);
		if (!!values.inputMode)
			values.inputMode = parseInt(values.inputMode);
		if (!!values.socdMode)
			values.socdMode = parseInt(values.socdMode);
		if (!!values.switchTpShareForDs4)
			values.switchTpShareForDs4 = parseInt(values.switchTpShareForDs4);
		if (!!values.forcedSetupMode)
			values.forcedSetupMode = parseInt(values.forcedSetupMode);
		if (!!values.lockHotkeys)
			values.lockHotkeys = parseInt(values.lockHotkeys);

		setButtonLabels({ swapTpShareLabels: (values.switchTpShareForDs4 === 1) && (values.inputMode === 4) });

		Object.keys(hotkeyFields).forEach(a => {
			const value = values[a];
			if (value) {
				values[a] = {
					action: parseInt(value.action),
					buttonsMask: parseInt(value.buttonsMask),
					auxMask: parseInt(value.auxMask)
				}
			};
		});
	}, [values, setValues]);

	return null;
};

export default function SettingsPage() {
	const { buttonLabels, setButtonLabels } = useContext(AppContext);
	const [saveMessage, setSaveMessage] = useState('');
	const [warning, setWarning] = useState({ show: false, acceptText: ''});

	const WARNING_CHECK_TEXT = "GP2040-CE";

	const handleWarningClose = async (accepted, values, setFieldValue) => {
		setWarning({ show: false, acceptText: ''});
		if (accepted) await saveSettings(values);
		else setFieldValue('forcedSetupMode', 0);
	};

	const setWarningAcceptText = (e) => {
		setWarning({ ...warning, acceptText: e.target.value });
	};

	const saveSettings = async (values) => {
		const success = await WebApi.setGamepadOptions(values);
		setSaveMessage(success ? 'Saved! Please Restart Your Device' : 'Unable to Save');
	};

	const onSuccess = async (values) => {
		if (values.forcedSetupMode > 1) { setWarning({ show: true, acceptText: ''}); }
		else { await saveSettings(values); }
	};

	const { buttonLabelType, swapTpShareLabels } = buttonLabels;

	const buttonLabelS1 = BUTTONS[buttonLabelType][(swapTpShareLabels && buttonLabelType === "ps4") ? "A2" : "S1"];
	const buttonLabelS2 = BUTTONS[buttonLabelType]["S2"];
	const buttonLabelA1 = BUTTONS[buttonLabelType]["A1"];

	return (
		<Formik validationSchema={schema} onSubmit={onSuccess} initialValues={{}}>
			{({
				handleSubmit,
				handleChange,
				values,
				errors,
				setFieldValue
			}) => console.log('errors', errors) || (
				<div>
					<Form noValidate onSubmit={handleSubmit}>
					<Section title="Settings">
						<Form.Group className="row mb-3">
							<Form.Label>Input Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="inputMode" className="form-select-sm" value={values.inputMode} onChange={handleChange} isInvalid={errors.inputMode}>
									{INPUT_MODES.map((o, i) => <option key={`button-inputMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.inputMode}</Form.Control.Feedback>
							</div>
							<div className="col-sm-3">
								{values.inputMode === PS4Mode && <Form.Check
									label="Switch Touchpad and Share"
									type="switch"
									name="switchTpShareForDs4"
									isInvalid={false}
									checked={Boolean(values.switchTpShareForDs4)}
									onChange={(e) => { setFieldValue("switchTpShareForDs4", e.target.checked ? 1 : 0); }}
								/>}
							</div>
						</Form.Group>
						<Form.Group className="row mb-3">
							<Form.Label>D-Pad Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="dpadMode" className="form-select-sm" value={values.dpadMode} onChange={handleChange} isInvalid={errors.dpadMode}>
									{DPAD_MODES.map((o, i) => <option key={`button-dpadMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.dpadMode}</Form.Control.Feedback>	
							</div>
						</Form.Group>
						<Form.Group className="row mb-3">
							<Form.Label>SOCD Cleaning Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="socdMode" className="form-select-sm" value={values.socdMode} onChange={handleChange} isInvalid={errors.socdMode}>
									{SOCD_MODES.map((o, i) => <option key={`button-socdMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.socdMode}</Form.Control.Feedback>
							</div>
						</Form.Group>
						<p>Note: PS4, PS3 and Nintendo Switch modes do not support setting SOCD Cleaning to Off and will default to Neutral SOCD Cleaning mode.</p>
						<Form.Group className="row mb-3">
							<Form.Label>Forced Setup Mode</Form.Label>
							<div className="col-sm-3">
								<Form.Select name="forcedSetupMode" className="form-select-sm" value={values.forcedSetupMode} onChange={handleChange} isInvalid={errors.forcedSetupMode}>
									{FORCED_SETUP_MODES.map((o, i) => <option key={`button-forcedSetupMode-option-${i}`} value={o.value}>{o.label}</option>)}
								</Form.Select>
								<Form.Control.Feedback type="invalid">{errors.forcedSetupMode}</Form.Control.Feedback>
							</div>
						</Form.Group>
					</Section>
					<Section title="Hotkey Settings">
						<div className="mb-3">The <strong>Fn</strong> slider provides a mappable Function button in the <NavLink exact="true" to="/pin-mapping">Pin Mapping</NavLink> page. By selecting the Fn slider option, the Function button must be held along with the selected hotkey settings.
						<br/>Additionally, select <strong>None</strong> from the dropdown to unassign any button.</div>
						{values.fnButtonPin === -1 && <div className="alert alert-warning">Function button is not mapped. The Fn slider will be disabled.</div> }
						<div id="Hotkeys"
							hidden={values.lockHotkeys}>
							{Object.keys(hotkeyFields).map((o, i) =>
								<Form.Group key={`hotkey-${i}`} className="row mb-3">
								<div className="col-sm-auto">
									<Form.Check name={`${o}.auxMask`} label="&nbsp;&nbsp;Fn" type="switch" className="form-select-sm" disabled={values.fnButtonPin === -1} checked={values[o] && !!(values[o]?.auxMask)} onChange={(e) => { setFieldValue(`${o}.auxMask`, e.target.checked ? 32768 : 0)}} isInvalid={errors[o] && errors[o]?.auxMask} />
									<Form.Control.Feedback type="invalid">{errors[o] && errors[o]?.action}</Form.Control.Feedback>
								</div>
								<span className="col-sm-auto">+</span>
								{BUTTON_MASKS.map(mask => ((values[o] && values[o]?.buttonsMask & mask.value) ?
									[<div className="col-sm-auto">
										<Form.Select
											name={`${o}.buttonsMask`}
											className="form-select-sm sm-1"
											groupClassName="mb-3"
											value={values[o] && (values[o]?.buttonsMask & mask.value)}
											error={errors[o] && errors[o]?.buttonsMask}
											isInvalid={errors[o] && errors[o]?.buttonsMask}
											onChange={(e) => { setFieldValue(`${o}.buttonsMask`, (values[o] && values[o]?.buttonsMask ^ mask.value) | e.target.value); }}>
												{BUTTON_MASKS.map((o, i2) => <option key={`hotkey-${i}-button${i2}`} value={o.value}>{o.label}</option>)}
										</Form.Select>	
									</div>, <span className="col-sm-auto">+</span>] : <></>))}
									<div className="col-sm-auto">
										<Form.Select
											name={`${o}.buttonsMask`}
											className="form-select-sm sm-1"
											groupClassName="mb-3"
											value={0}
											onChange={(e) => { setFieldValue(`${o}.buttonsMask`, (values[o] && values[o]?.buttonsMask) | e.target.value); }}>
												{BUTTON_MASKS.map((o, i2) => <option key={`hotkey-${i}-buttonZero-${i2}`} value={o.value}>{o.label}</option>)}
										</Form.Select>
									</div>
									<span className="col-sm-auto">=</span>
									<div className="col-sm-auto">
										<Form.Select name={`${o}.action`} className="form-select-sm" value={values[o] && values[o]?.action} onChange={handleChange} isInvalid={errors[o] && errors[o]?.action}>
											{HOTKEY_ACTIONS.map((o, i) => <option key={`hotkey-action-${i}`} value={o.value}>{o.label}</option>)}
										</Form.Select>
										<Form.Control.Feedback type="invalid">{errors[o] && errors[o]?.action}</Form.Control.Feedback>
									</div>
								</Form.Group>
							)}
						</div>
						<Form.Check
							label="Lock Hotkeys"
							type="switch"
							id="LockHotkeys"
							reverse
							isInvalid={false}
							checked={Boolean(values.lockHotkeys)}
							onChange={(e) => { setFieldValue("lockHotkeys", e.target.checked ? 1 : 0); }}
						/>	
					</Section>
					<Button type="submit">Save</Button>
					{saveMessage ? <span className="alert">{saveMessage}</span> : null}
					<FormContext  setButtonLabels={setButtonLabels}/>
					</Form>
					<Modal size="lg" show={warning.show} onHide={handleWarningClose}>
						<Modal.Header closeButton>
							<Modal.Title>Forced Setup Mode Warning</Modal.Title>
						</Modal.Header>
						<Modal.Body>
							<div className='mb-3'>
								If you reboot to Controller mode after saving, you will no longer have access to the web-config.
								Please type "<strong>{WARNING_CHECK_TEXT}</strong>" below to unlock the Save button if you fully acknowledge this and intend it.
								Clicking on Dismiss will revert this setting which then is to be saved.
							</div>
							<Form.Control value={warning.acceptText} onChange={setWarningAcceptText}></Form.Control>
						</Modal.Body>
						<Modal.Footer>
							<Button disabled={warning.acceptText != WARNING_CHECK_TEXT} variant="warning" onClick={() => handleWarningClose(true, values)}>
								Save
							</Button>
							<Button variant="primary" onClick={() => handleWarningClose(false, values, setFieldValue)}>
								Dismiss
							</Button>
						</Modal.Footer>
					</Modal>
				</div>
			)}
		</Formik>
	);
}
