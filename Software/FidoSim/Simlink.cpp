#include "Simlink.h"


Simlink::Simlink() : emitter(50), robot(100, 80) {
	irVal = motors.motorOne = motors.motorTwo = 0;
	micVal = 30;
	visVal = batVal = 50;
	tempVal = 50;

	imu.accel.xComp = imu.accel.yComp = imu.accel.zComp = 0;
	imu.compass.xComp = imu.compass.yComp = imu.compass.zComp = 0;
	imu.gyro.xComp = imu.gyro.yComp = imu.gyro.zComp = 0;

	click = false;
	keepWindowsOpen = true;

	mainWindowThread = std::thread(&Simlink::mainWindowHandler, this);
	kinematicWindowThread = std::thread(&Simlink::kinematicWindowHandler, this);
}

Simlink::~Simlink() {
	closeWindow();
}

void Simlink::closeWindow() {
	keepWindowsOpen = false;

	if(mainWindowThread.joinable() == true) mainWindowThread.join();
	if(kinematicWindowThread.joinable() == true) kinematicWindowThread.join();
}

void Simlink::mainWindowHandler() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Initialize main window
	mainWindow.create(sf::VideoMode(1200, 900), "Fido Simulator", sf::Style::Default, settings);

	sf::Texture texture;
	if (!texture.loadFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/background.png")) {
		exit(EXIT_FAILURE);
	} background = sf::Sprite(texture);

	sf::Font font;
	if (!font.loadFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/sansation.ttf")) {
		exit(EXIT_FAILURE);
	} sf::Text text("Hello SFML", font, 50);
	text.setColor(sf::Color::Black);

	sf::Music music;
	if (!music.openFromFile("C:/Users/Michael/Documents/Fido/Software/FidoSim/nice_music.ogg")) {
		exit(EXIT_FAILURE);
	} music.play();

	while (keepWindowsOpen == true) {
		updateMainWindow();
	}

	mainWindow.close();
}

void Simlink::updateMainWindow() {
	if (mainWindow.isOpen() == false) return;

	sf::Event event;
	while (mainWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mainWindow.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) mainWindow.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				click = true;
				cx = event.mouseButton.x;
				cy = event.mouseButton.y;
			} break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				click = false;
			break;
		case sf::Event::MouseMoved:
			if (click) {
				cx = event.mouseMove.x;
				cy = event.mouseMove.y;
			} break;
		default:
			break;
		}
	}

	if (click) {
		if (cx > 160 && cx < 220 && cy > 20 && cy < 280)
			imu.accel.xComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 20 && cy < 280)
			imu.accel.yComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 20 && cy < 280)
			imu.accel.zComp = -((cy - 20) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 320 && cy < 580)
			imu.gyro.xComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 320 && cy < 580)
			imu.gyro.yComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 320 && cy < 580)
			imu.gyro.zComp = -((cy - 320) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 620 && cy < 880)
			imu.compass.xComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 620 && cy < 880)
			imu.compass.yComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 620 && cy < 880)
			imu.compass.zComp = -((cy - 620) - 130) / 1.28;

		if (cx > 780 && cx < 850 && cy > 620 && cy < 880)
			batVal = (880 - cy) / 2.6;
		else if (cx > 890 && cx < 960 && cy > 620 && cy < 880)
			irVal = (880 - cy) / 2.6;
		else if (cx > 1000 && cx < 1070 && cy > 620 && cy < 880)
			visVal = (880 - cy) / 2.6;
		else if (cx > 1110 && cx < 1180 && cy > 620 && cy < 880)
			micVal = (880 - cy) / 2.6;
		else if (cx > 500 && cx < 700 && cy > 800 && cy < 880)
			tempVal = (cx - 500) / 2;

	}

	sf::CircleShape ledCirc(40);
	ledCirc.setFillColor(sf::Color(led.r, led.g, led.b));
	ledCirc.setPosition(675, 625);
	ledCirc.setOutlineThickness(5);
	ledCirc.setOutlineColor(sf::Color(0, 0, 0));

	sf::RectangleShape slide(sf::Vector2f(40, 30));
	slide.setFillColor(sf::Color(0, 0, 0));

	mainWindow.clear();
	mainWindow.draw(background);
	mainWindow.draw(ledCirc);

	slide.setPosition(170, 133 - imu.accel.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(280, 133 - imu.accel.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(390, 133 - imu.accel.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(168, 435 - imu.gyro.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(278, 435 - imu.gyro.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(388, 435 - imu.gyro.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(166, 732 - imu.compass.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(276, 732 - imu.compass.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(386, 732 - imu.compass.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(797, 858 - batVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(906, 858 - irVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(1016, 858 - visVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(1126, 858 - micVal*2.5);
	mainWindow.draw(slide);

	sf::RectangleShape horizontalSlide(sf::Vector2f(30, 40));
	horizontalSlide.setFillColor(sf::Color(0, 0, 0));

	horizontalSlide.setPosition(500 + tempVal*2, 835);
	mainWindow.draw(horizontalSlide);

	sf::RectangleShape mOneLine(sf::Vector2f(20, abs(motors.motorOne) * 2));
	sf::RectangleShape mTwoLine(sf::Vector2f(20, abs(motors.motorTwo) * 2));
	mOneLine.setFillColor(sf::Color(0, 0, 0));
	mTwoLine.setFillColor(sf::Color(0, 0, 0));

	if (motors.motorOne > 0) {
		mOneLine.setPosition(680, 230 - motors.motorOne * 2);
		mainWindow.draw(mOneLine);
	}
	else if (motors.motorOne < 0) {
		mOneLine.setPosition(680, 370);
		mainWindow.draw(mOneLine);
	} if (motors.motorTwo > 0) {
		mTwoLine.setPosition(980, 230 - motors.motorTwo * 2);
		mainWindow.draw(mTwoLine);
	}
	else if (motors.motorTwo < 0) {
		mTwoLine.setPosition(980, 370);
		mainWindow.draw(mTwoLine);
	}

	std::string pText = "Vol: " + std::to_string(piezo.volume)
		+ "\nFreq: " + std::to_string(piezo.frequency);

	/**
	sf::Font font;
	font.loadFromFile("times.ttf");
	sf::Text piezoText("Hello guyz",font);
	piezoText.setCharacterSize(50);
	piezoText.setPosition(1000,500);
	mainWindow.draw(piezoText);
	**/

	mainWindow.display();
}

void Simlink::kinematicWindowHandler() {
	// Initialize kinematic window
	kinematicWindow.create(sf::VideoMode(1000, 800), "Fido Kinematic Simulator");

	while (keepWindowsOpen == true) {
		updateKinematicWindow();
	}

	kinematicWindow.close();
}

void Simlink::updateKinematicWindow() {
	while (kinematicWindow.isOpen()) {
		sf::Event event;
		while (kinematicWindow.pollEvent(event))
			if (event.type == sf::Event::Closed) kinematicWindow.close();

		/// emitter mouse stuff (copy this)
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			emitter.set(sf::Mouse::getPosition(kinematicWindow));
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			emitter.bye();

		int mLeft, mRight;

		/// testing of kinematic model
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mLeft = 100;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mLeft = -100;
		else mLeft = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) mRight = 100;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mRight = -100;
		else mRight = 0;

		TDVect emitSense = emitter.sense(robot);
		std::cout << "x:" << emitSense.xComp << ",y:" << emitSense.yComp << "\n";
		imu.compass = emitSense;

		robot.goAccel(mLeft, mRight);

		kinematicWindow.clear(sf::Color(255, 255, 255));
		kinematicWindow.draw(robot);
		kinematicWindow.draw(emitter);
		kinematicWindow.display();
		sf::sleep(sf::milliseconds(25));
	}
}

TDVect Simlink::getAccel() {
    return imu.accel;
}

TDVect Simlink::getGyro() {
    return imu.gyro;
}

TDVect Simlink::getCompass() {
    return imu.compass;
}

int Simlink::getMicrophone() {
    return micVal;
}

int Simlink::getIR() {
    return irVal;
}

int Simlink::getVis() {
    return visVal;
}

int Simlink::getBattery() {
    return batVal;
}

void Simlink::setLED(int r, int g, int b, int i) {
    led.r = r * i/100;
    led.g = g * i/100;
    led.b = b * i/100;
}

void Simlink::setMotors(int motorOne, int motorTwo) {
    motors.motorOne = motorOne;
    motors.motorTwo = motorTwo;
}

void Simlink::chirp(int volume, int frequency) {
    piezo.frequency = frequency;
    piezo.volume = volume;
    int endTime = 200 + clock();
    while(clock() != endTime);
    piezo.volume = 0;
}

int Simlink::getTemperature() {
	return tempVal;
}