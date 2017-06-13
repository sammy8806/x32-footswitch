#ifndef X32TYPE_H
#define X32TYPE_H

#include <QObject>
#include <QRegExp>

#define X32_INTERNAL
// class X32Console;

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

enum X32BoolState {
    OFF,
    ON
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

struct ChannelConfig {
    QString name;
    qint16 icon;
    X32LcdColors color;
    qint16 source; // Off, In 01..32, Aux 01..06, USB L..R, FX 1L..4R, Bus 01..16
};

struct ChannelDelay {
    X32BoolState on;
    float time; // 0.300 .. 500.0 (0.1)
};

struct ChannelPreamp {
    float trim; // -18.0 .. 18.0 (0.25) # digital sources only ?
    X32BoolState invert;
    X32BoolState hpon; // phantom power;
    X32HpSlope slope;
    float hpf; // linf 20.0 .. 400.0 (101)
};

struct FxFilter {
    X32BoolState on;
    X32GateFilterType type;
    float f; // logf 20 .. 20000 (201)
};

struct FxGate {
    X32BoolState on;
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
    X32BoolState on;
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
    X32BoolState automix; // named "auto"
    FxFilter filter;
};

struct ChannelInsert {
    X32BoolState on;
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
    X32BoolState on;
    QList<EqSingle> eq;
};

struct ChannelMixSend {
    X32BoolState on;
    float level; // level -90 .. 10 (161)
};

struct ChannelMixSendStereo : ChannelMixSend {
    float pan; // linf -100 .. 100 (2)
    X32InsertType type;
};

struct ChannelMix {
    X32BoolState on;
    float fader; // level 0 .. 1 (1/1024)   dB
    X32BoolState st; // stereo?
    float pan; // linf -100 .. 100 (2)
    X32BoolState mono; // Enabled LCR/Mono?
    float mlevel; // level -90 .. 10 (161) // LCR/Mono level?

    QList<ChannelMixSend> mix; // Beware of ChannelMixSendStereo
};

// Only effective on CH01..08
struct ChannelAutomix {
    X32AutomixGroup group;
    float weight; // linf -12 .. 12 (0.5)
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
