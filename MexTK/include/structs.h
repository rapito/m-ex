#ifndef MEX_H_STRUCTS
#define MEX_H_STRUCTS

/* 
 *  defines typedefs for structs used throughout m-ex
 */

// Basic Data Structs
typedef struct Vec2 Vec2;
typedef struct Vec3 Vec3;
typedef struct Vec4 Vec4;

// OS
typedef struct OSInfo OSInfo;
typedef struct OSCalendarTime OSCalendarTime;
typedef struct CARDStat CARDStat;
typedef struct OSAlarm OSAlarm;
typedef struct CARDFileInfo CARDFileInfo;
typedef struct RGB565 RGB565;
typedef struct MTHPlayParam MTHPlayParam;
typedef struct MTHHeader MTHHeader;
typedef struct MTHPlayback MTHPlayback;
typedef struct JPEGHeader JPEGHeader;

// GX
typedef struct GXColor GXColor;
typedef struct GXRenderModeObj GXRenderModeObj;
typedef struct GXPipe GXPipe;

// Audio
typedef struct BGMData BGMData;
typedef struct VPB VPB;
typedef struct AXLive AXLive;

// HSD Objects
typedef struct HSD_Obj HSD_Obj;
typedef struct GOBJ GOBJ;
typedef struct GOBJProc GOBJProc;
typedef struct GOBJList GOBJList;
typedef struct GXList GXList;
typedef struct JOBJ JOBJ;
typedef struct JOBJDesc JOBJDesc;
typedef struct MatAnimJointDesc MatAnimJointDesc;
typedef struct DOBJ DOBJ;
typedef struct TOBJ TOBJ;
typedef struct AOBJ AOBJ;
typedef struct MOBJ MOBJ;
typedef struct WOBJ WOBJ;
typedef struct COBJ COBJ;
typedef struct COBJDesc COBJDesc;
typedef struct _HSD_ImageDesc _HSD_ImageDesc;
typedef struct _HSD_LightPoint _HSD_LightPoint;
typedef struct _HSD_LightPointDesc _HSD_LightPointDesc;
typedef struct _HSD_LightSpot _HSD_LightSpot;
typedef struct _HSD_LightSpotDesc _HSD_LightSpotDesc;
typedef struct _HSD_LightAttn _HSD_LightAttn;
typedef struct LObjDesc LObjDesc;
typedef struct LightAnim LightAnim;
typedef struct LightGroup LightGroup;
typedef struct LOBJ LOBJ;
typedef struct HSD_Fog HSD_Fog;
typedef struct JOBJSet JOBJSet;

// Archive
typedef struct HSD_Archive HSD_Archive;
typedef struct MapHead MapHead;

// Stage
typedef struct Stage Stage;
typedef struct StageOnGO StageOnGO;
typedef struct MapData MapData;
typedef struct MapDesc MapDesc;
typedef struct StageFile StageFile;
typedef struct grGroundParam grGroundParam;
typedef struct GrDesc GrDesc;
typedef struct GrExtLookup GrExtLookup;
typedef struct LineHazardDesc LineHazardDesc;
typedef struct MapItemDesc MapItemDesc;
typedef struct LineRange LineRange;
typedef struct GeneralPoints GeneralPoints;
typedef struct GeneralPointsInfo GeneralPointsInfo;
typedef struct MapCollLink MapCollLink;
typedef struct MapGObjDesc MapGObjDesc;

// Match
typedef struct MatchInit MatchInit;
typedef struct Match Match;
typedef struct MatchOffscreen MatchOffscreen;
typedef struct MatchHUD MatchHUD;
typedef struct MatchHUDElement MatchHUDElement;
typedef struct MatchHUDStock MatchHUDStock;
typedef struct MatchCamera MatchCamera;
typedef struct CameraBox CameraBox;
typedef struct MatchStandings MatchStandings;
typedef struct TeamStandings TeamStandings;
typedef struct PlayerStandings PlayerStandings;
typedef struct ExclamData ExclamData;

// Text
typedef struct Text Text;
typedef struct TextAllocInfo TextAllocInfo;

// DevText
typedef struct DevText DevText;

// Effects
typedef struct Effect Effect;
typedef struct Particle Particle;
typedef struct Particle2 Particle2;
typedef struct ptclGen ptclGen;
typedef struct GeneratorAppSRT GeneratorAppSRT;

// Color
typedef struct ColAnimDesc ColAnimDesc;
typedef struct ColorOverlay ColorOverlay;

// Item
//typedef struct ItemModelDesc;
//typedef struct ItemDesc;
//typedef struct itCommonData;
//typedef struct itPublicData;
typedef struct ItemStateDesc ItemStateDesc;
typedef struct ItemData ItemData;
typedef struct ItemState ItemState;
typedef struct SpawnItem SpawnItem;
typedef struct itData itData;
typedef struct itCommonAttr itCommonAttr;
typedef struct itHit itHit;
typedef struct ItHurt ItHurt;
typedef struct ItDynamics ItDynamics;
typedef struct ItDynamicBoneset ItDynamicBoneset;

// Boneset
typedef struct DynamicBoneset DynamicBoneset;

// Fighter
typedef struct FighterData FighterData;
typedef struct FighterBone FighterBone;
typedef struct Playerblock Playerblock;
typedef struct PlayerData PlayerData;
typedef struct FtState FtState;
typedef struct SubactionHeader SubactionHeader;
typedef struct ftHit ftHit;
typedef struct HitVictim HitVictim;
typedef struct FtHurt FtHurt;
typedef struct FtCoin FtCoin;
typedef struct ReflectDesc ReflectDesc;
typedef struct ShieldDesc ShieldDesc;
typedef struct AbsorbDesc AbsorbDesc;
typedef struct AfterImageDesc AfterImageDesc;
typedef struct FtDmgLog FtDmgLog;
typedef struct CPULeaderLog CPULeaderLog;
typedef struct CPU CPU;
typedef struct FtCollDesc FtCollDesc;
typedef struct FtSFX FtSFX;
typedef struct FtAction FtAction;
typedef struct ftData ftData;
typedef struct FtDmgVibrateDesc FtDmgVibrateDesc;
typedef struct ftCommonData ftCommonData;
typedef struct FtMultiJumpDesc FtMultiJumpDesc;
typedef struct ftChkDevice ftChkDevice;
typedef struct FtSymbolLookup FtSymbolLookup;
typedef struct FtSymbols FtSymbols;
typedef struct FtDatNameLookup FtDatNameLookup;
typedef struct FtKindDesc FtKindDesc;
typedef struct FtParts FtParts;
typedef struct FtPartsDesc FtPartsDesc;
typedef struct FtPartsVis FtPartsVis;
typedef struct FtPartsLookup FtPartsLookup;
typedef struct FtPartsVisLookup FtPartsVisLookup;
typedef struct FtSymbolLookup FtSymbolLookup;
typedef struct FtDynamicBoneset FtDynamicBoneset;
typedef struct FtDynamicRoot FtDynamicRoot;
typedef struct FtDynamicHit FtDynamicHit;
typedef struct DynamicsDesc DynamicsDesc;
typedef struct DynamicsHitDesc DynamicsHitDesc;
typedef struct DynamicsBehave DynamicsBehave;
typedef struct IKParam IKParam;
typedef struct FtVis FtVis;
typedef struct ftDynamics ftDynamics;

// Fighter States
typedef struct FtCliffCatch FtCliffCatch;
typedef struct FtDamage FtDamage;

// CSS
typedef struct CSSBackup CSSBackup;
typedef struct MnSelectChrDataTable MnSelectChrDataTable;
typedef struct VSMinorData VSMinorData;
typedef struct SSSMinorData SSSMinorData;
typedef struct CSSCursor CSSCursor;
typedef struct CSSPuck CSSPuck;
typedef struct MnSlChrKindData MnSlChrKindData;
typedef struct MnSlChrData MnSlChrData;
typedef struct MnSlChrIcon MnSlChrIcon;
typedef struct MnSlChrDoor MnSlChrDoor;
typedef struct MnSlChrTag MnSlChrTag;
typedef struct MnSlChrTagData MnSlChrTagData;
typedef struct MnSlChrKOStar MnSlChrKOStar;

// Memcard
typedef struct Memcard Memcard;
typedef struct MemcardWork MemcardWork;
typedef struct MemcardUnk MemcardUnk;
typedef struct MemcardSave MemcardSave;
typedef struct MemcardInfo MemcardInfo;
typedef struct SnapshotInfo SnapshotInfo;
typedef struct SnapshotList SnapshotList;
typedef struct MemSnapIconData MemSnapIconData;
typedef struct MemSaveIconData MemSaveIconData;
typedef struct Rules1 Rules1;
typedef struct Rules4 Rules4;

// Collision
typedef struct CollData CollData;
typedef struct ECBSize ECBSize;
typedef struct CollLineInfo CollLineInfo;
typedef struct CollLine CollLine;
typedef struct CollVert CollVert;
typedef struct CollLineUnk CollLineUnk;
typedef struct CollDataStage CollDataStage;
typedef struct CollGroupDesc CollGroupDesc;
typedef struct CollGroup CollGroup;

// HSD
typedef struct HSD_Material HSD_Material;
typedef struct HSD_Pad HSD_Pad;
typedef struct HSD_Pads HSD_Pads;
typedef struct HSD_Update HSD_Update;
typedef struct HSD_VI HSD_VI;
typedef struct HSD_ObjAllocData HSD_ObjAllocData;

// Scene
typedef struct MajorScene MajorScene;
typedef struct MinorScene MinorScene;
typedef struct SceneInfo SceneInfo;
typedef struct ScDataVS ScDataVS;
typedef struct ScDataRst ScDataRst;

// Preload
typedef struct Preload Preload;
typedef struct PreloadChar PreloadChar;
typedef struct PreloadEntry PreloadEntry;
typedef struct PreloadLookup PreloadLookup;
typedef struct PreloadAllocData PreloadAllocData;
typedef struct PreloadTable PreloadTable;

// Kirby
typedef struct FtVarKirby FtVarKirby;

// Custom
typedef struct Stc_icns Stc_icns;
typedef struct MEXPlaylist MEXPlaylist;

#endif