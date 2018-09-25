#pragma once
#include "SimpleAudioEngine.h"
#include <string>
using namespace CocosDenshion;
using namespace std;

class AudioM {
public:
	static void preloadAudio();
	static void playBGM(string s, bool loop = true);
	static void playSound(string s, bool loop = false);

	static void turnBgm(bool on);
	static void turnSound(bool on);
	static bool hasBgm();
	static bool hasSound();
	static void resumeBGM();

	const static string mySuffix;
	// const static string mySuffix = ".ogg";

private:
	static string curMusic;
	static bool openBgm;
	static bool openSound;
};