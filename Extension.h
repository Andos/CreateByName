
class Extension
{
public:

    LPRDATA rdPtr;
    LPRH    rhPtr;

    Edif::Runtime Runtime;

    static const int MinimumBuild = 252;
	static const int Version = 1;

    static const int OEFLAGS = 0;
    static const int OEPREFS = 0;
    
    static const int WindowProcPriority = 100;

    Extension(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr);
    ~Extension();
	
	int oiListItemSize;


    /*  Add any data you want to store in your extension to this class
        (eg. what you'd normally store in rdPtr)

        Unlike rdPtr, you can store real C++ objects with constructors
        and destructors, without having to call them manually or store
        a pointer.
    */

	bool isHWA;
	UINT codepage;

    /// Actions
	void CreateAtPos(LPTSTR objName, eventParam* evtParam, int layer);
	void CreateAtPosXY(LPTSTR objName, int x, int y, int layer);
	void CreateBackdropAtPos(LPTSTR objName, long position, int type, int layer);
	void CreateBackdropAtPosXY(LPTSTR objName, int x, int y, int type, int layer);

	/// Expressions
	TCHAR* NameFromFixed(int fixed);

    // These are called if there's no function linked to an ID
	void Action(int ID, LPRDATA rdPtr, long param1, long param2);
	long Condition(int ID, LPRDATA rdPtr, long param1, long param2);
	long Expression(int ID, LPRDATA rdPtr, long param);


	short Handle();
	short Display();

	short Pause();
	short Continue();

	bool Save(HANDLE File);
	bool Load(HANDLE File);

};