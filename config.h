#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QStringList>

static const float playerSizeMultiplier = 0.015;
static const float playerStartRectY = 0.8;
static const float playerSpeedMultiplier = 0.006;

static const char* companionFilePath = "../../image/MatthiasDerStern.png";
static const char* playerShipFilePath = "../../image/playerShip.png";
static const char* enemyShipFilePath1 = "../../image/enemyShip.png";
static const char* enemyShipFilePath2 = "../../image/enemyShip2.png";
static const char* enemyShipFilePath3 = "../../image/enemyShip3.png";
static const char* BulletRedFilePath = "../../image/bullet_red.png";
static const char* BulletYellowFilePath = "../../image/bullet_yellow.png";
static const char* BulletBlueFilePath = "../../image/bullet_blue.png";
static const char* BulletGreenFilePath = "../../image/bullet_green.png";

// Meteoriten
static const char* redMeteorite = "../../image/MeteoriteNewRED";
static const char* greenMeteorite = "../../image/MeteoriteNewGreen";
static const char* blueMeteorite = "../../image/MeteoriteNewBLUE";
static const char* yellowMeteorite = "../../image/MeteoriteNewYellow";

static const char* menuImagePath = "../../image/menu.png";
static const char* winImagePath = "../../image/win.png";
static const char* loseImagePath = "../../image/lose.png";

static const char* bg1MusicPath = "../../music/bg1.ogg";
static const char* bg2MusicPath = "../../music/bg2.ogg";
static const char* bg3MusicPath = "../../music/bg3.ogg";
static const char* bg4MusicPath = "../../music/bg4.ogg";
static const char* bg5MusicPath = "../../music/bg5.ogg";
static const char* bg6MusicPath = "../../music/bg6.ogg";
static const char* bg7MusicPath = "../../music/bg7.ogg";
static const char* bg8MusicPath = "../../music/bg8.ogg";
static const char* bg9MusicPath = "../../music/bg9.ogg";
static const char* bg10MusicPath = "../../music/bg10.ogg";
static const char* bg11MusicPath = "../../music/bg11.ogg";
static const char* bg12MusicPath = "../../music/bg12.ogg";
static const char* bg13MusicPath = "../../music/bg13.ogg";
static const char* bg14MusicPath = "../../music/bg14.ogg";
static const char* bg15MusicPath = "../../music/bg15.ogg";
static const char* bg16MusicPath = "../../music/bg16.ogg";
static const char* bg17MusicPath = "../../music/bg17.ogg";
static const char* bg18MusicPath = "../../music/bg18.ogg";
static const char* bg19MusicPath = "../../music/bg19.ogg";
static const char* bg20MusicPath = "../../music/bg20.ogg";
static const char* bg21MusicPath = "../../music/bg21.ogg";
static const char* bg22MusicPath = "../../music/bg22.ogg";
static const char* bg23MusicPath = "../../music/bg23.ogg";
static const char* bg24MusicPath = "../../music/bg24.ogg";
static const char* bg25MusicPath = "../../music/bg25.ogg";
static const char* bg26MusicPath = "../../music/bg26.ogg";
static const char* bg27MusicPath = "../../music/bg27.ogg";
static const char* menuMusicPath = "../../music/menu.mp3";
static const char* winMusicPath = "../../music/win.mp3";
static const char* loseMusicPath = "../../music/lose.mp3";
static const char* hitSoundPath = "../../music/hit.wav";
static const char* enterSoundPath = "../../music/enter.wav";
static const char* shootSoundPath = "../../music/shoot.wav";
static const char* cartiSoundPath = "../../music/carti.mp3";
static const char* swampSoundPath = "../../music/swampizzo.mp3";
static const char* lashSoundPath = "../../music/lash.mp3";
static const char* lash2SoundPath = "../../music/lash2.mp3";

QStringList const RadioSongs1 = {
    "Now Playing: It's Going Down Now",
    "Now Playing: Yousei Teikoku - Zetsubou plantation",
    "Now Playing: Last Surprise",
    "Now Playing: Pursuing My True Self",
    "Now Playing: Deadlock - Main Menu Theme Music",
    "Now Playing: Whereabouts of the Heart",
    "Now Playing: Five Nights at Freddy's 2 Song",
    "Now Playing: Survive the Night",
    "Now Playing: Can You Hear The Music",
    "Now Playing: Asymmetry - Reol",
    "Now Playing: DAN DA DAN Opening | Otonoke by Creepy Nuts",
    "Now Playing: CHAINSAW MAN  Opening",
    "Now Playing: LittleBigPlanet 2 Soundtrack - Sleepy Head",
    "Now Playing: 【MV】M@GICAL☆CURE! LOVE ♥ SHOT! / SAWTOWNE feat. Hatsune Miku",
};

QStringList const RadioSongs2 = {
    "Now Playing: ITSUDEMO I LOVE YOU / FUKKIRETA",
    "Now Playing: Aria Math but breakcore",
    "Now Playing: xi - Ascension to Heaven",
    "Now Playing: xi - Blue Zenith",
    "Now Playing: xi - Freedom Dive",
    "Now Playing: Kurokotei - Galaxy Collapse",
    "Now Playing: DJ Sharpnel - Cyber Inductance 1.4 Rate",
};

QStringList const RadioSongs3 = {
    "Now Playing: Tyler The Creator - EARFQUAKE",
    "Now Playing: Tyler The Creator - See You Again",
    "Now Playing: Kanye West - Dark Fantasy",
    "Now Playing: Kanye West - Runaway",
    "Now Playing: Invincible Credits Theme",
    "Now Playing: Deadlock - Baroness Hotel Music",
};

static const char* winVideoPath = "../../video/win.mp4";
static const char* winVideo2Path = "../../video/win2.mp4";

static const char* loseVideoPath = "../../video/unravel.mp4";

static const char* bg1VideoPath = "../../video/bg1.mp4";
static const char* bg2VideoPath = "../../video/bg2.mp4";
static const char* bg3VideoPath = "../../video/bg3.mp4";
static const char* bg4VideoPath = "../../video/bg4.mp4";
static const char* bg5VideoPath = "../../video/bg5.mp4";
static const char* bg6VideoPath = "../../video/bg6.mp4";

static const char* bgLebronVideoPath = "../../video/bglebron.mp4";
static const char* bgLebronVideoPath2 = "../../video/bglebron2.mp4";
static const char* bgLebronVideoPath3 = "../../video/bglebron3.mp4";
static const char* bgLebronVideoPath4 = "../../video/bglebron4.mp4";
static const char* bgLebronVideoPath5 = "../../video/bglebron5.mp4";
static const char* bgKobeVideoPath = "../../video/bgkobe.mp4";
static const char* bgKobeShaqVideoPath = "../../video/bgkobeshaq.mp4";
static const char* bgJordanVideoPath = "../../video/bgjordan.mp4";
static const char* bgLebronHeatVideoPath = "../../video/bglebronheat.mp4";

static const char* cubeVideoPath = "../../video/cube.mp4";

#endif // CONFIG_H
