#ifndef X32TYPE_H
#define X32TYPE_H

#include <QObject>
#include <QRegExp>

#define X32_INTERNAL
// class X32Console;

#include <osc/composer/OscMessageComposer.h>
#include <osc/reader/OscMessage.h>
#include <oscudpsocket.h>

class X32ConsoleAbstract : public QObject {
    Q_OBJECT

public:
    X32ConsoleAbstract(QObject *parent = nullptr) : QObject(parent) {

    }

    virtual void setSocket(OscUdpSocket *socket);
    virtual void sendMessage(OscMessageComposer msg);

    virtual void removeMessage(OscMessage& msg);

protected:
    OscUdpSocket *socket;
    QVector<OscMessage*> *dataPool;
};

#define CHAN_NORMAL_MAX 31
#define CHAN_AUX_MAX 39
#define CHAN_FX_MAX 47
#define CHAN_BUS_MAX 63
#define CHAN_MATRIX_MAX 69
#define CHAN_MAINLR 70
#define CHAN_MAINMC 71
#define CHAN_DCA_MAX 79
#define CHAN_MUTEGROUP_MAX 85

enum X32LcdColors {
    Off,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    OffInvert,
    RedInvert,
    GreenInvert,
    YellowInvert,
    BlueInvert,
    MagentaInvert,
    CyanInvert,
    WhiteInvert,
};

enum X32HpSlope {
    HP12 = 12,
    HP18 = 18,
    HP24 = 24
};

enum X32GateMode {
    EXP2,
    EXP3,
    EXP4,
    GATE,
    DUCK
};

enum X32GateFilterType {
    LC6, LC12, HC6, HC12, V1, V2, V3, V5, V10
};

enum X32DynamicMode {
    COMP, EXP
};

enum X32DynamicDetermination {
    PEAK, RMS
};

enum X32DynamicEnv {
    LIN, LOG
};

enum X32FxInsertPos {
    Pre, Post
};

enum X32DynamicRatio {
    R1_1,
    R1_3,
    R1_5,
    R2,
    R2_5,
    R3,
    R4,
    R5,
    R7,
    R10,
    R20,
    R100
};

enum X32InsertSelect {
    NoInsert,
    FX1L, FX1R, FX2L, FX2R, FX3L, FX3R, FX4L, FX4R,
    FX5L, FX5R, FX6L, FX6R, FX7L, FX7R, FX8L, FX8R,
    AUX1, AUX2, AUX3, AUX4, AUX5, AUX6
};

enum X32EqType {
    LowCut, LowShelve, Parametric, Visual, HighShelve, HighCut
};

enum X32InsertType {
    INLC,
    LeftEQ,
    RightEQ,
    PRE,
    POST,
    Group
};

enum X32AutomixGroup {
    NoGroup, X, Y
};

enum X32ConfigBtn : char {
    BtnJumpToPage = 'P',
    BtnMute = 'O',
    BtnInsert = 'I',
    BtnEffect = 'X',
    BtnMidi = 'M', // MidiPush & MidiToggle
    BtnRemote = 'R',
    BtnRecall = 'S', // 3 Types of Recall (Scene=0, Snippet=2, Cue=4)
    BtnUsbRecorder = 'T',
    BtnAutomixEnable = 'A'
};

enum X32ConfigRotary : char {
    RotFader = 'F',
    RotPan = 'P',
    RotSend = 'S',
    RotEffect = 'X',
    RotMidi = 'M',
    RotRemote = 'R',
    RotChanSelect = 'D'
};

struct ChannelConfig {
    QString name;
    qint16 icon;
    X32LcdColors color;
    qint16 source; // Off, In 01..32, Aux 01..06, USB L..R, FX 1L..4R, Bus 01..16
};

struct ChannelDelay {
    bool on;
    float time; // 0.300 .. 500.0 (0.1)
};

struct ChannelPreamp {
    float trim; // -18.0 .. 18.0 (0.25) # digital sources only ?
    bool invert;
    bool hpon; // phantom power;
    X32HpSlope slope;
    float hpf; // linf 20.0 .. 400.0 (101)
};

struct FxFilter {
    bool on;
    X32GateFilterType type;
    float f; // logf 20 .. 20000 (201)
};

struct FxGate {
    bool on;
    X32GateMode mode;
    float thr;   // -80.00 .. 0    (0.5) dB linf
    float range;   // 3.00 .. 60   (1.0) dB linf
    float attack;  // 0.00 .. 120  (1.0) ms linf
    float hold;    // 0.02 .. 2000 (101) ms logf
    float release; // 5.00 .. 4000 (101) ms logf
    qint8 keysrc;  // Off, In 01..32, Aux 01..06, USB L..R, FX 1L..4R, Bus 01..16
    FxFilter filter;
};

struct ChannelDynamic {
    bool on;
    X32DynamicMode mode;
    X32DynamicDetermination det;
    X32DynamicEnv env;
    float thr; // linf -60 .. 0 (0.5) dB
    X32DynamicRatio ratio;
    float knee; // linf 0 .. 5 (1)
    float mgain; // linf 0 .. 24 (0.5) dB
    float attack; // linf 0 .. 120 (1) ms
    float hold; // logf 0.02 .. 2000 (101) ms
    float release; // logf 5 .. 4000 (101) ms
    X32FxInsertPos pos;
    qint8 keysrc;  // Off, In 01..32, Aux 01..06, USB L..R, FX 1L..4R, Bus 01..16
    float mix; // linf 0 .. 100 (5) %
    bool automix; // named "auto"
    FxFilter filter;
};

struct ChannelInsert {
    bool on;
    X32FxInsertPos pos;
    X32InsertSelect sel;
};

struct EqSingle {
    X32EqType type;
    float f; // logf 20 .. 20000 (201) Hz   # Frequency
    float g; // linf -15 .. 15 (0.25) dB    # Gain
    float q; // logf 10 .. 72 (0.3)         # Width
};

struct EQ {
    bool on;
    QList<EqSingle> eq;
};

struct ChannelMixSend {
    bool on;
    float level; // level -90 .. 10 (161)
};

struct ChannelMixSendStereo : ChannelMixSend {
    float pan; // linf -100 .. 100 (2)
    X32InsertType type;
};

struct ChannelMix {
    bool on;
    float fader; // level 0 .. 1 (1/1024)   dB
    bool st; // stereo?
    float pan; // linf -100 .. 100 (2)
    bool mono; // Enabled LCR/Mono?
    float mlevel; // level -90 .. 10 (161) // LCR/Mono level?

    QList<ChannelMixSend> mix; // Beware of ChannelMixSendStereo
};

// Only effective on CH01..08
struct ChannelAutomix {
    X32AutomixGroup group;
    float weight; // linf -12 .. 12 (0.5)
};

struct ChLink {
    bool link1_2;
    bool link3_4;
    bool link5_6;
    bool link7_8;
    bool link9_10;
    bool link11_12;
    bool link13_14;
    bool link15_16;
    bool link17_18;
    bool link19_20;
    bool link21_22;
    bool link23_24;
    bool link25_26;
    bool link27_28;
    bool link29_30;
    bool link31_32;
};

struct AuxLink {
    bool link1_2;
    bool link3_4;
    bool link5_6;
    bool link7_8;
};

struct FxLink {
    bool link1_2;
    bool link3_4;
    bool link5_6;
    bool link7_8;
};

struct BusLink {
    bool link1_2;
    bool link3_4;
    bool link5_6;
    bool link7_8;
    bool link9_10;
    bool link11_12;
    bool link13_14;
    bool link15_16;
};

struct MatrixLink {
    bool link1_2;
    bool link3_4;
    bool link5_6;
};

struct MuteConfig {
    bool mute1;
    bool mute2;
    bool mute3;
    bool mute4;
    bool mute5;
    bool mute6;
};

struct LinkConfig {
    bool hadly; // Delay + HA
    bool eq; // EQ
    bool dyn; // Dynamics
    bool fdrmute; // Mute/Fader
};

struct MonoConfig {
    bool mode; // LR+M / LCR
    bool link; // M/C depends on MainLR
};

struct UserctrlButton {
    X32ConfigBtn type;
    QString data;
};

typedef QMap<qint8, UserctrlButton> UserctrlBankData;

struct UserctrlBank {
    QChar bank;
    UserctrlBankData *data;
};

class X32Type : public QObject
{
    Q_OBJECT
public:
    X32Type(QObject *parent = nullptr) : QObject(parent) {

    }

signals:

public slots:
};

#endif // X32TYPE_H
