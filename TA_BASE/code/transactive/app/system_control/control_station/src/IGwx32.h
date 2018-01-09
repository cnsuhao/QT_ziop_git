// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard
// IGwx32 wrapper class

class IGwx32 : public COleDispatchDriver
{
public:
    IGwx32() {} // Calls COleDispatchDriver default constructor
    IGwx32(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    IGwx32(const IGwx32& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

    // Attributes
public:

    // Operations
public:

    // IGwx32 methods
public:
    void ShowWindow()
    {
        InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void RefreshWindow()
    {
        InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void BringWindowToTop()
    {
        InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void HideWindow()
    {
        InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void StartRuntime()
    {
        InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void StopRuntime()
    {
        InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL ToggleRuntime()
    {
        BOOL result;
        InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void ExitApplication()
    {
        InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void MinimizeWindow()
    {
        InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void MaximizeWindow()
    {
        InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void RestoreWindow()
    {
        InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    LPDISPATCH GetVisibleObjectFromName(LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, objectName);
        return result;
    }
    LPDISPATCH CreateEllipse(float left, float top, float width, float height, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, left, top, width, height, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    LPDISPATCH CreateRectangle(float left, float top, float width, float height, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName, BOOL rounded)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR VTS_BOOL;
        InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, left, top, width, height, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName, rounded);
        return result;
    }
    LPDISPATCH CreateText(float x, float y, LPCTSTR text, long alignment, BOOL stretchText, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_R4 VTS_R4 VTS_BSTR VTS_I4 VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, x, y, text, alignment, stretchText, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    LPDISPATCH CreateSymbol(LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, objectName);
        return result;
    }
    BOOL UngroupSymbol(LPCTSTR objectName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objectName);
        return result;
    }
    BOOL DeleteObject(LPCTSTR objectName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objectName);
        return result;
    }
    void DeselectAllObjects()
    {
        InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    LPDISPATCH CreatePolyline(VARIANT& vertices, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_VARIANT VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &vertices, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    LPDISPATCH CreateArc(long arcType, float centerX, float centerY, float radiusX, float radiusY, float startAngle, float endAngle, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, arcType, centerX, centerY, radiusX, radiusY, startAngle, endAngle, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    void SetDisplayDimensions(long width, long height)
    {
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, width, height);
    }
    void GetDisplayDimensions(long* width, long* height)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4;
        InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, width, height);
    }
    void SetViewDimensions(long left, long top, long width, long height)
    {
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetViewDimensions(long* left, long* top, long* width, long* height)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4;
        InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    LPDISPATCH GetDynamicObjectFromName(LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x1d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, objectName);
        return result;
    }
    BOOL SetAliasDefinition(LPCTSTR aliasName, LPCTSTR newDefinition)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, aliasName, newDefinition);
        return result;
    }
    LPDISPATCH GetPointObjectFromName(LPCTSTR pointName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x1f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, pointName);
        return result;
    }
    void SetWindowDimensionsPixels(long left, long top, long width, long height)
    {
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4;
        InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void SetWindowDimensionsPercent(float left, float top, float width, float height)
    {
        static BYTE parms[] = VTS_R4 VTS_R4 VTS_R4 VTS_R4;
        InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetWindowDimensionsPercent(float* left, float* top, float* width, float* height)
    {
        static BYTE parms[] = VTS_PR4 VTS_PR4 VTS_PR4 VTS_PR4;
        InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetWindowDimensionsPixels(long* left, long* top, long* width, long* height)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4;
        InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    BOOL DeleteDynamic(LPCTSTR objectName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objectName);
        return result;
    }
    BOOL FileOpen(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    BOOL FileSave()
    {
        BOOL result;
        InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL FileSaveAs(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x27, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    BOOL FileNew()
    {
        BOOL result;
        InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL FilePrint()
    {
        BOOL result;
        InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    LPDISPATCH GetHeadObject()
    {
        LPDISPATCH result;
        InvokeHelper(0x2a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
        return result;
    }
    BOOL SetHeadObject(LPCTSTR objectName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objectName);
        return result;
    }
    void CloseAllPopupWindows()
    {
        InvokeHelper(0x2c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    LPDISPATCH OpenPopupWindow(LPCTSTR filename, BOOL modal, BOOL center, BOOL hidden)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL;
        InvokeHelper(0x2d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, modal, center, hidden);
        return result;
    }
    BOOL ClosePopupWithTitle(LPCTSTR titleSubstring)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, titleSubstring);
        return result;
    }
    LPDISPATCH GetOpenPopupWithTitle(LPCTSTR titleSubstring)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x2f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, titleSubstring);
        return result;
    }
    LPDISPATCH CreateSizeDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, long sizeType, BOOL clip, float startSize, float endSize)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL VTS_R4 VTS_R4;
        InvokeHelper(0x30, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, sizeType, clip, startSize, endSize);
        return result;
    }
    LPDISPATCH CreateLocationDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, float offsetX, float offsetY, BOOL slider, BOOL tracking, short numberOfDetents, BOOL continuousUpdate)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R4 VTS_R4 VTS_BOOL VTS_BOOL VTS_I2 VTS_BOOL;
        InvokeHelper(0x31, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, offsetX, offsetY, slider, tracking, numberOfDetents, continuousUpdate);
        return result;
    }
    LPDISPATCH CreateRotationDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, float startAngle, float endAngle, float pivotX, float pivotY, BOOL clockwise, BOOL dial, BOOL tracking, short numberOfDetents, BOOL continuousUpdate)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL VTS_BOOL VTS_BOOL VTS_I2 VTS_BOOL;
        InvokeHelper(0x32, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, startAngle, endAngle, pivotX, pivotY, clockwise, dial, tracking, numberOfDetents, continuousUpdate);
        return result;
    }
    LPDISPATCH CreateHideDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, BOOL hideWhenTrue, BOOL disableObject)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL;
        InvokeHelper(0x33, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, hideWhenTrue, disableObject);
        return result;
    }
    LPDISPATCH CreateFlashDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, BOOL hideObject, BOOL flashWhenTrue, BOOL altStateWhenOff, BOOL changeFill, BOOL changeLine, BOOL changeShadow, unsigned long altFillColor, unsigned long altLineColor, unsigned long altShadowColor)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4 VTS_UI4;
        InvokeHelper(0x34, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, hideObject, flashWhenTrue, altStateWhenOff, changeFill, changeLine, changeShadow, altFillColor, altLineColor, altShadowColor);
        return result;
    }
    LPDISPATCH CreateAnalogColorDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, BOOL changeFill, BOOL changeLine, BOOL changeShadow, unsigned long startFillColor, unsigned long endFillColor, unsigned long startLineColor, unsigned long endLineColor, unsigned long startShadowColor, unsigned long endShadowColor, BOOL defaultColorAbove, BOOL defaultColorBelow)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_UI4 VTS_BOOL VTS_BOOL;
        InvokeHelper(0x35, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, changeFill, changeLine, changeShadow, startFillColor, endFillColor, startLineColor, endLineColor, startShadowColor, endShadowColor, defaultColorAbove, defaultColorBelow);
        return result;
    }
    LPDISPATCH CreatePickDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, long pickAction, long pickType, long executionTrigger, long mouseButton, BOOL initiallySelected, LPCTSTR groupName, LPCTSTR filename, BOOL modal, BOOL center, LPCTSTR value1, LPCTSTR value2, LPCTSTR value3)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BSTR VTS_BSTR VTS_BSTR;
        InvokeHelper(0x36, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, pickAction, pickType, executionTrigger, mouseButton, initiallySelected, groupName, filename, modal, center, value1, value2, value3);
        return result;
    }
    LPDISPATCH CreateButton(long buttonType, float x, float y, LPCTSTR label, long alignment, BOOL stretchText, BOOL isFilled, unsigned long fillColor, unsigned long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, unsigned long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4 VTS_R4 VTS_R4 VTS_BSTR VTS_I4 VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4 VTS_I4 VTS_I4 VTS_BOOL VTS_UI4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x37, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, buttonType, x, y, label, alignment, stretchText, isFilled, fillColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    LPDISPATCH CreateTimedate(LPCTSTR textObjectName, LPCTSTR dynamicObjectName, long formatType, LPCTSTR timeFormat, LPCTSTR dateFormat)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR;
        InvokeHelper(0x38, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, textObjectName, dynamicObjectName, formatType, timeFormat, dateFormat);
        return result;
    }
    LPDISPATCH CreateProcessPoint(LPCTSTR textObjectName, LPCTSTR dynamicObjectName, long dataType, BOOL update, BOOL dataEntry, BOOL hasInitialValue, VARIANT& initialValue, LPCTSTR format)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL VTS_BOOL VTS_BOOL VTS_VARIANT VTS_BSTR;
        InvokeHelper(0x39, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, textObjectName, dynamicObjectName, dataType, update, dataEntry, hasInitialValue, &initialValue, format);
        return result;
    }
    LPDISPATCH CreateAnimator(LPCTSTR symbolName, LPCTSTR dynamicObjectName, BOOL animateWhenTrue, BOOL visibleWhenOff, BOOL currentFrameWhenOff)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL;
        InvokeHelper(0x3a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, symbolName, dynamicObjectName, animateWhenTrue, visibleWhenOff, currentFrameWhenOff);
        return result;
    }
    LPDISPATCH CreateAnalogSelector(LPCTSTR symbolName, LPCTSTR dynamicObjectName, BOOL hiddenWhenAbove, BOOL hiddenWhenBelow)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL;
        InvokeHelper(0x3b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, symbolName, dynamicObjectName, hiddenWhenAbove, hiddenWhenBelow);
        return result;
    }
    LPDISPATCH CreateDigitalColorDynamic(LPCTSTR visibleObjectName, LPCTSTR dynamicObjectName, BOOL changeColorWhenTrue, BOOL changeFill, BOOL changeLine, BOOL changeShadow, unsigned long fillColor, unsigned long lineColor, unsigned long shadowColor, LPCTSTR dataSource)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4 VTS_UI4 VTS_BSTR;
        InvokeHelper(0x3c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, visibleObjectName, dynamicObjectName, changeColorWhenTrue, changeFill, changeLine, changeShadow, fillColor, lineColor, shadowColor, dataSource);
        return result;
    }
    LPDISPATCH CreateDigitalSelector(LPCTSTR symbolName, LPCTSTR dynamicObjectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x3d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, symbolName, dynamicObjectName);
        return result;
    }
    CString GetFileName()
    {
        CString result;
        InvokeHelper(0x3e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    BOOL IsRuntimeMode()
    {
        BOOL result;
        InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void GetClientDimensionsPixels(long* left, long* top, long* width, long* height)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4;
        InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    BOOL PopCurrentSymbol()
    {
        BOOL result;
        InvokeHelper(0x41, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL PopAllCurrentSymbol()
    {
        BOOL result;
        InvokeHelper(0x42, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    long GetNumberOfTopLevelVisibles()
    {
        long result;
        InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    LPDISPATCH GetVisibleObjectFromIndex(long index)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x44, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, index);
        return result;
    }
    BOOL OpenTagBrowser(long hWndParent, BSTR* tagName)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_PBSTR;
        InvokeHelper(0x45, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, hWndParent, tagName);
        return result;
    }
    BOOL SelectPaletteColor(unsigned long* SelectedColor)
    {
        BOOL result;
        static BYTE parms[] = VTS_PUI4;
        InvokeHelper(0x46, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SelectedColor);
        return result;
    }
    void DuplicateSelection()
    {
        InvokeHelper(0x47, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL QueryRanges(LPCTSTR dataSource, double* lowRange, double* highRange)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8;
        InvokeHelper(0x48, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dataSource, lowRange, highRange);
        return result;
    }
    BOOL QueryDataType(LPCTSTR dataSource, long* dataType)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_PI4;
        InvokeHelper(0x49, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dataSource, dataType);
        return result;
    }
    BOOL SetRuntimeUpdateRate(long updateRate)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x4a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, updateRate);
        return result;
    }
    BOOL SetTooltipProperties(BOOL ShowDynamicTips, BOOL ShowDynamicObjectName, BOOL ShowDynamicObjectDescription, BOOL ShowDataSourceName, BOOL ShowDataSourceValue, BOOL ShowStaticTooltips, BOOL ShowStaticObjectName, BOOL ShowStaticObjectDescription)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
        InvokeHelper(0x4b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ShowDynamicTips, ShowDynamicObjectName, ShowDynamicObjectDescription, ShowDataSourceName, ShowDataSourceValue, ShowStaticTooltips, ShowStaticObjectName, ShowStaticObjectDescription);
        return result;
    }
    BOOL SetRuntimeFocusProperties(BOOL ShowHandCursor, BOOL ShowFocusRectangle, unsigned long fillColor, unsigned long BorderColor)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_UI4 VTS_UI4;
        InvokeHelper(0x4c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ShowHandCursor, ShowFocusRectangle, fillColor, BorderColor);
        return result;
    }
    LPDISPATCH GwxGetVbaProject()
    {
        LPDISPATCH result;
        InvokeHelper(0x4d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
        return result;
    }
    long GetFrameWindowHandle()
    {
        long result;
        InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    CString GetObjectTypeName()
    {
        CString result;
        InvokeHelper(0x4f, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    LPDISPATCH OpenEmbeddedWindow(LPCTSTR filename, BOOL center, BOOL hidden)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL;
        InvokeHelper(0x50, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, center, hidden);
        return result;
    }
    BOOL DisplayBack()
    {
        BOOL result;
        InvokeHelper(0x51, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL DisplayForward()
    {
        BOOL result;
        InvokeHelper(0x52, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void LogicalPointToScreen(long* x, long* y)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4;
        InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
    }
    void ViewHome()
    {
        InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void ViewUnzoom()
    {
        InvokeHelper(0x55, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void ViewPan(long offsetX, long offsetY)
    {
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x56, DISPATCH_METHOD, VT_EMPTY, NULL, parms, offsetX, offsetY);
    }
    void ViewZoomPercent(float newZoom)
    {
        static BYTE parms[] = VTS_R4;
        InvokeHelper(0x57, DISPATCH_METHOD, VT_EMPTY, NULL, parms, newZoom);
    }
    void ViewFitToWindow()
    {
        InvokeHelper(0x58, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void ViewShowWholeDisplay()
    {
        InvokeHelper(0x59, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void ViewBoxZoom()
    {
        InvokeHelper(0x5a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void ViewZoomCustomDialog()
    {
        InvokeHelper(0x5b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    LPDISPATCH GetParentDisplay()
    {
        LPDISPATCH result;
        InvokeHelper(0x5c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
        return result;
    }
    BOOL SetAliases(LPCTSTR commandString)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x5d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, commandString);
        return result;
    }
    BOOL SetAliasesFromFile(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x5e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    long GetOpenPopupCount()
    {
        long result;
        InvokeHelper(0x5f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    LPDISPATCH GetOpenPopupByIndex(long popupIndex)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x60, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, popupIndex);
        return result;
    }
    BOOL IsPopupWindow()
    {
        BOOL result;
        InvokeHelper(0x61, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL IsEmbeddedPopupWindow()
    {
        BOOL result;
        InvokeHelper(0x62, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void CloseWindow()
    {
        InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL FileOpenSetAliases(LPCTSTR filename, long aliasCommandType, LPCTSTR aliasCommandString)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR;
        InvokeHelper(0x64, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename, aliasCommandType, aliasCommandString);
        return result;
    }
    LPDISPATCH OpenPopupWinSetAliases(LPCTSTR filename, BOOL modal, BOOL center, BOOL hidden, long aliasCommandType, LPCTSTR aliasCommandString)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_I4 VTS_BSTR;
        InvokeHelper(0x65, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, modal, center, hidden, aliasCommandType, aliasCommandString);
        return result;
    }
    LPDISPATCH OpenEmbeddedWinSetAliases(LPCTSTR filename, BOOL center, BOOL hidden, long aliasCommandType, LPCTSTR aliasCommandString)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_BSTR;
        InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, center, hidden, aliasCommandType, aliasCommandString);
        return result;
    }
    BOOL OpenSetAliasesDialog(long aliasCommandType, LPCTSTR aliasCommandString, LPCTSTR objectName)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR;
        InvokeHelper(0x67, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, aliasCommandType, aliasCommandString, objectName);
        return result;
    }
    void LogicalPointToClient(long* x, long* y)
    {
        static BYTE parms[] = VTS_PI4 VTS_PI4;
        InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
    }
    LPDISPATCH CreateBitmap(LPCTSTR filename, float left, float top, float width, float height, BOOL isTransparent, long transparentColor, long lineColor, long lineWidth, long lineStyle, BOOL hasShadow, long shadowColor, long edgeStyle, BOOL isHidden, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_I4 VTS_BOOL VTS_BSTR;
        InvokeHelper(0x69, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, left, top, width, height, isTransparent, transparentColor, lineColor, lineWidth, lineStyle, hasShadow, shadowColor, edgeStyle, isHidden, objectName);
        return result;
    }
    LPDISPATCH CreateMetafile(LPCTSTR filename, float left, float top, float width, float height, LPCTSTR objectName)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_BSTR;
        InvokeHelper(0x6a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, left, top, width, height, objectName);
        return result;
    }
    BOOL FileSaveAsType(LPCTSTR filename, short displayType)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_I2;
        InvokeHelper(0x6b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename, displayType);
        return result;
    }
    BOOL TestCustomSecurityItem(LPCTSTR customString)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x6c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, customString);
        return result;
    }
    CString GetFilePath()
    {
        CString result;
        InvokeHelper(0x6d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    CString GetWorkingDirectory()
    {
        CString result;
        InvokeHelper(0x6e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    BOOL ExportBitmapCaptureOfView(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x6f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    LPDISPATCH CreateStateField(LPCTSTR textObjectName, LPCTSTR dynamicObjectName, BOOL update, BOOL dataEntry, BOOL hasInitialValue, VARIANT& initialValue, LPCTSTR stateConfiguration, LPCTSTR defaultState)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_VARIANT VTS_BSTR VTS_BSTR;
        InvokeHelper(0x70, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, textObjectName, dynamicObjectName, update, dataEntry, hasInitialValue, &initialValue, stateConfiguration, defaultState);
        return result;
    }
    BOOL SetGradientFill(long style, unsigned long color1, unsigned long color2, BOOL isTwoColor, float brightness, BOOL reverseColors, short steps, float horizontalOffset, float verticalOffset)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_UI4 VTS_UI4 VTS_BOOL VTS_R4 VTS_BOOL VTS_I2 VTS_R4 VTS_R4;
        InvokeHelper(0x71, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, style, color1, color2, isTwoColor, brightness, reverseColors, steps, horizontalOffset, verticalOffset);
        return result;
    }
    void ClearGradientFill()
    {
        InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL SetLayerOverrideHide(LPCTSTR layerName, BOOL hidden)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL;
        InvokeHelper(0x73, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layerName, hidden);
        return result;
    }
    BOOL ToggleLayerOverrideHide(LPCTSTR layerName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x74, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, layerName);
        return result;
    }
    void SetModifiedFlag(BOOL Modified)
    {
        static BYTE parms[] = VTS_BOOL;
        InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Modified);
    }
    BOOL IsModified()
    {
        BOOL result;
        InvokeHelper(0x76, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL MoveSelectionToFront()
    {
        BOOL result;
        InvokeHelper(0x77, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL MoveSelectionToBack()
    {
        BOOL result;
        InvokeHelper(0x78, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL MoveSelectionForward()
    {
        BOOL result;
        InvokeHelper(0x79, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL MoveSelectionBackward()
    {
        BOOL result;
        InvokeHelper(0x7a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL GetSystemWideLanguage(BSTR* Language, BSTR* LanguageSubset, long* LocaleID, BOOL* SynchronizeLCID)
    {
        BOOL result;
        static BYTE parms[] = VTS_PBSTR VTS_PBSTR VTS_PI4 VTS_PBOOL;
        InvokeHelper(0x7b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Language, LanguageSubset, LocaleID, SynchronizeLCID);
        return result;
    }
    BOOL SetSystemWideLanguage(LPCTSTR Language, LPCTSTR LanguageSubset, long LocaleID, BOOL SynchronizeLCID)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL;
        InvokeHelper(0x7c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Language, LanguageSubset, LocaleID, SynchronizeLCID);
        return result;
    }
    void CreateLayer(LPCTSTR layerName)
    {
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x7d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, layerName);
    }
    void RemoveLayer(LPCTSTR layerName)
    {
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, layerName);
    }
    long GetFileVersion()
    {
        long result;
        InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    long SetFileVersion(long NewFileVersion)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, parms, NewFileVersion);
        return result;
    }
    BOOL GetFileContainsVBA()
    {
        BOOL result;
        InvokeHelper(0x81, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void EnableRuntimeEvents(BOOL LeftButton, BOOL MiddleButton, BOOL RightButton, BOOL MouseMove, BOOL Keyboard)
    {
        static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL;
        InvokeHelper(0x82, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LeftButton, MiddleButton, RightButton, MouseMove, Keyboard);
    }
    long ReplaceTag(LPCTSTR OldSubstring, LPCTSTR NewSubstring)
    {
        long result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x83, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OldSubstring, NewSubstring);
        return result;
    }
    long ReplaceHost(LPCTSTR OldHostName, LPCTSTR NewHostName)
    {
        long result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x84, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OldHostName, NewHostName);
        return result;
    }
    LPDISPATCH GetVisibleObjectFromPosition(long x, long y, BOOL GoIntoSymbol)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL;
        InvokeHelper(0x85, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, x, y, GoIntoSymbol);
        return result;
    }
    long ReplaceFilePath(LPCTSTR OldSubstring, LPCTSTR NewSubsring)
    {
        long result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x86, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OldSubstring, NewSubsring);
        return result;
    }
    long ReplaceHostEx(LPCTSTR OldHostNameSubstring, LPCTSTR NewHostNameSubstring, BOOL MatchCase, BOOL MatchWholeWord)
    {
        long result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL;
        InvokeHelper(0x87, DISPATCH_METHOD, VT_I4, (void*)&result, parms, OldHostNameSubstring, NewHostNameSubstring, MatchCase, MatchWholeWord);
        return result;
    }
    BOOL SetTransparency(BOOL EnableTransparency, unsigned long transparentColor, BOOL EnableAlpha, short AlphaBlending)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL VTS_UI4 VTS_BOOL VTS_I2;
        InvokeHelper(0x88, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, EnableTransparency, transparentColor, EnableAlpha, AlphaBlending);
        return result;
    }
    BOOL GetTransparency(BOOL* TransparencyEnabled, unsigned long* transparentColor, BOOL* AlphaEnabled, short* AlphaBlending)
    {
        BOOL result;
        static BYTE parms[] = VTS_PBOOL VTS_PUI4 VTS_PBOOL VTS_PI2;
        InvokeHelper(0x89, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, TransparencyEnabled, transparentColor, AlphaEnabled, AlphaBlending);
        return result;
    }
    BOOL SetGASThemes(LPCTSTR Themes)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0x8a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Themes);
        return result;
    }
    CString GetGASThemes()
    {
        CString result;
        InvokeHelper(0x8b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    BOOL InsertLibraryObject(LPDISPATCH libraryObject)
    {
        BOOL result;
        static BYTE parms[] = VTS_DISPATCH;
        InvokeHelper(0x8c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, libraryObject);
        return result;
    }
    BOOL ConvertGwxSymbolToLibraryObject(LPDISPATCH GwxSymbol, LPDISPATCH* libraryObject)
    {
        BOOL result;
        static BYTE parms[] = VTS_DISPATCH VTS_PDISPATCH;
        InvokeHelper(0x8d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, GwxSymbol, libraryObject);
        return result;
    }
    BOOL InsertLibraryObjectByName(LPCTSTR category, LPCTSTR symbol)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x8e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, category, symbol);
        return result;
    }
    BOOL InsertLibraryObjectByIndex(LPCTSTR category, long index)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_I4;
        InvokeHelper(0x8f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, category, index);
        return result;
    }
    BOOL InsertOleObject(LPCTSTR progID, LPCTSTR objName)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR;
        InvokeHelper(0x90, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, progID, objName);
        return result;
    }
    BOOL SelectPaletteColorRGB(VARIANT* red, VARIANT* green, VARIANT* blue)
    {
        BOOL result;
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0x91, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, red, green, blue);
        return result;
    }
    long GetLoadTabsCount()
    {
        long result;
        InvokeHelper(0x92, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    BOOL GetLoadTab(long index, BSTR* label, BSTR* File, long* AliasCmdType, BSTR* AliasCmdString, BSTR* GlobalAliasCmdString)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_PBSTR VTS_PBSTR VTS_PI4 VTS_PBSTR VTS_PBSTR;
        InvokeHelper(0x93, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, index, label, File, AliasCmdType, AliasCmdString, GlobalAliasCmdString);
        return result;
    }
    CString GetEventScriptText(long Event)
    {
        CString result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x94, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Event);
        return result;
    }
    long GetEventScriptType(long Event)
    {
        long result;
        static BYTE parms[] = VTS_I4;
        InvokeHelper(0x95, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Event);
        return result;
    }
    void SetEventScriptText(long Event, LPCTSTR text)
    {
        static BYTE parms[] = VTS_I4 VTS_BSTR;
        InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Event, text);
    }
    void SetEventScriptType(long Event, long Type)
    {
        static BYTE parms[] = VTS_I4 VTS_I4;
        InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Event, Type);
    }
    BOOL SetRuntimeFocus(LPCTSTR objectName, BOOL bDataEntry)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL;
        InvokeHelper(0x98, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objectName, bDataEntry);
        return result;
    }
    BOOL SetRuntimeFocusNext(BOOL bWrap, BOOL bDataEntry)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL VTS_BOOL;
        InvokeHelper(0x99, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bWrap, bDataEntry);
        return result;
    }
    BOOL SetRuntimeFocusPrev(BOOL bWrap, BOOL bDataEntry)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL VTS_BOOL;
        InvokeHelper(0x9a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bWrap, bDataEntry);
        return result;
    }
    LPDISPATCH GetRuntimeFocus()
    {
        LPDISPATCH result;
        InvokeHelper(0x9b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
        return result;
    }
    void CancelRuntimeFocus()
    {
        InvokeHelper(0x9c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL GetVCRMode()
    {
        BOOL result;
        InvokeHelper(0x9d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void SetVCRMode(BOOL Enable)
    {
        static BYTE parms[] = VTS_BOOL;
        InvokeHelper(0x9e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enable);
    }
    BOOL VCRControlPanel(BOOL Show)
    {
        BOOL result;
        static BYTE parms[] = VTS_BOOL;
        InvokeHelper(0x9f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Show);
        return result;
    }
    VARIANT GetLocalAliases(LPCTSTR DisplayName)
    {
        VARIANT result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xa0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, DisplayName);
        return result;
    }
    VARIANT GetLocalVariables(LPCTSTR DisplayName)
    {
        VARIANT result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xa1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, DisplayName);
        return result;
    }
    VARIANT GetSimulationVariables(LPCTSTR DisplayName)
    {
        VARIANT result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xa2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, DisplayName);
        return result;
    }
    void EnableRuntimeZoomAndPan(BOOL Enable)
    {
        static BYTE parms[] = VTS_BOOL;
        InvokeHelper(0xa3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enable);
    }
    BOOL IsEnabledRuntimeZoomAndPan()
    {
        BOOL result;
        InvokeHelper(0xa4, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void SetWorkingDirectory(LPCTSTR NewDirectory)
    {
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xa5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NewDirectory);
    }
    void SetPrintWhiteBackground(BOOL WhiteBackground, long PrintArea)
    {
        static BYTE parms[] = VTS_BOOL VTS_I4;
        InvokeHelper(0xa6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WhiteBackground, PrintArea);
    }
    BOOL GetPrintWhiteBackground()
    {
        BOOL result;
        InvokeHelper(0xa7, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    long GetPrintViewArea()
    {
        long result;
        InvokeHelper(0xa8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
        return result;
    }
    void SetDesignModeEvents(short Enable)
    {
        static BYTE parms[] = VTS_I2;
        InvokeHelper(0xa9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enable);
    }
    short GetDesignModeEvents()
    {
        short result;
        InvokeHelper(0xaa, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
        return result;
    }
    void GetDisplayDimensions2(VARIANT* width, VARIANT* height)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xac, DISPATCH_METHOD, VT_EMPTY, NULL, parms, width, height);
    }
    void GetViewDimensions2(VARIANT* left, VARIANT* top, VARIANT* width, VARIANT* height)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xad, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetWindowDimensionsPercent2(VARIANT* left, VARIANT* top, VARIANT* width, VARIANT* height)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xae, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetWindowDimensionsPixels2(VARIANT* left, VARIANT* top, VARIANT* width, VARIANT* height)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xaf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    void GetClientDimensionsPixels2(VARIANT* left, VARIANT* top, VARIANT* width, VARIANT* height)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, left, top, width, height);
    }
    BOOL OpenTagBrowser2(long hWndParent, VARIANT* tagName)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_PVARIANT;
        InvokeHelper(0xb1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, hWndParent, tagName);
        return result;
    }
    BOOL SelectPaletteColor2(VARIANT* SelectedColor)
    {
        BOOL result;
        static BYTE parms[] = VTS_PVARIANT;
        InvokeHelper(0xb2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, SelectedColor);
        return result;
    }
    BOOL QueryRanges2(LPCTSTR dataSource, VARIANT* lowRange, VARIANT* highRange)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dataSource, lowRange, highRange);
        return result;
    }
    BOOL QueryDataType2(LPCTSTR dataSource, VARIANT* dataType)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_PVARIANT;
        InvokeHelper(0xb4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dataSource, dataType);
        return result;
    }
    void LogicalPointToScreen2(VARIANT* x, VARIANT* y)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
    }
    void LogicalPointToClient2(VARIANT* x, VARIANT* y)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
    }
    BOOL GetSystemWideLanguage2(VARIANT* Language, VARIANT* LanguageSubset, VARIANT* LocaleID, VARIANT* SynchronizeLCID)
    {
        BOOL result;
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Language, LanguageSubset, LocaleID, SynchronizeLCID);
        return result;
    }
    BOOL GetTransparency2(VARIANT* TransparencyEnabled, VARIANT* transparentColor, VARIANT* AlphaEnabled, VARIANT* AlphaBlending)
    {
        BOOL result;
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, TransparencyEnabled, transparentColor, AlphaEnabled, AlphaBlending);
        return result;
    }
    BOOL GetLoadTab2(long index, VARIANT* label, VARIANT* File, VARIANT* AliasCmdType, VARIANT* AliasCmdString, VARIANT* GlobalAliasCmdString)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xb9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, index, label, File, AliasCmdType, AliasCmdString, GlobalAliasCmdString);
        return result;
    }
    CString GetVCRNode()
    {
        CString result;
        InvokeHelper(0xba, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    void SetVCRNode(LPCTSTR NewNode)
    {
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xbb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NewNode);
    }
    void GetDisplayPropertiesDisabledColors(VARIANT* ShowAlternateColors, VARIANT* fillColor, VARIANT* lineColor, VARIANT* shadowColor)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xbc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ShowAlternateColors, fillColor, lineColor, shadowColor);
    }
    void SetDisplayPropertiesDisabledColors(BOOL ShowAlternateColors, unsigned long fillColor, unsigned long lineColor, unsigned long shadowColor)
    {
        static BYTE parms[] = VTS_BOOL VTS_UI4 VTS_UI4 VTS_UI4;
        InvokeHelper(0xbd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ShowAlternateColors, fillColor, lineColor, shadowColor);
    }
    void GetDisplayPropertiesFailureColors(VARIANT* ShowPointFailColors, VARIANT* fillColor, VARIANT* lineColor, VARIANT* shadowColor)
    {
        static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
        InvokeHelper(0xbe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ShowPointFailColors, fillColor, lineColor, shadowColor);
    }
    void SetDisplayPropertiesFailureColors(BOOL ShowPointFailColors, unsigned long fillColor, unsigned long lineColor, unsigned long shadowColor)
    {
        static BYTE parms[] = VTS_BOOL VTS_UI4 VTS_UI4 VTS_UI4;
        InvokeHelper(0xbf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ShowPointFailColors, fillColor, lineColor, shadowColor);
    }
    BOOL ExportToIntermediatFile(LPCTSTR LicenseKey, LPCTSTR OutputFileName, BOOL ConvertMetafilesToSymbol)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL;
        InvokeHelper(0xc0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, LicenseKey, OutputFileName, ConvertMetafilesToSymbol);
        return result;
    }
    BOOL ResetDisplayHistory()
    {
        BOOL result;
        InvokeHelper(0xc1, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    CString GetFileHistory()
    {
        CString result;
        InvokeHelper(0xc2, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    void SetFileHistory(LPCTSTR HistoryDescriptionString)
    {
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xc3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, HistoryDescriptionString);
    }
    BOOL TemplateApply(LPCTSTR TemplateFile)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR;
        InvokeHelper(0xc4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, TemplateFile);
        return result;
    }
    BOOL TemplateRemove()
    {
        BOOL result;
        InvokeHelper(0xc5, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL ResolveAliasesPermanently()
    {
        BOOL result;
        InvokeHelper(0xc6, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL FileOpenEx(long BlockingCall, LPCTSTR filename, long aliasCommandType, LPCTSTR aliasCommandString, LPCTSTR GlobalAliasCmdString)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR;
        InvokeHelper(0xc7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, BlockingCall, filename, aliasCommandType, aliasCommandString, GlobalAliasCmdString);
        return result;
    }
    LPDISPATCH OpenPopupWinSetAliasesEx(LPCTSTR filename, BOOL modal, BOOL center, BOOL hidden, long aliasCommandType, LPCTSTR aliasCommandString, LPCTSTR GlobalAliasCmdString)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_I4 VTS_BSTR VTS_BSTR;
        InvokeHelper(0xc8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, modal, center, hidden, aliasCommandType, aliasCommandString, GlobalAliasCmdString);
        return result;
    }
    LPDISPATCH OpenEmbeddedWinSetAliasesEx(LPCTSTR filename, BOOL center, BOOL hidden, long aliasCommandType, LPCTSTR aliasCommandString, LPCTSTR GlobalAliasCmdString)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_BSTR VTS_BSTR;
        InvokeHelper(0xc9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, filename, center, hidden, aliasCommandType, aliasCommandString, GlobalAliasCmdString);
        return result;
    }
    BOOL UpdateSharedObject(long updateScope, VARIANT& unused, LPCTSTR keyword, long keepProperties)
    {
        BOOL result;
        static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_BSTR VTS_I4;
        InvokeHelper(0xca, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, updateScope, &unused, keyword, keepProperties);
        return result;
    }
    BOOL ClosePopupWithTitleEx(LPCTSTR titleSubstring, long FromPosition, long SameLength)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4;
        InvokeHelper(0xcb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, titleSubstring, FromPosition, SameLength);
        return result;
    }

    // IGwx32 properties
public:
    BOOL GetRedraw()
    {
        BOOL result;
        GetProperty(0x1, VT_BOOL, (void*)&result);
        return result;
    }
    void SetRedraw(BOOL propVal)
    {
        SetProperty(0x1, VT_BOOL, propVal);
    }
    long GetScaleMode()
    {
        long result;
        GetProperty(0x2, VT_I4, (void*)&result);
        return result;
    }
    void SetScaleMode(long propVal)
    {
        SetProperty(0x2, VT_I4, propVal);
    }
    unsigned long GetBackgroundColor()
    {
        unsigned long result;
        GetProperty(0x3, VT_UI4, (void*)&result);
        return result;
    }
    void SetBackgroundColor(unsigned long propVal)
    {
        SetProperty(0x3, VT_UI4, propVal);
    }
    BOOL GetSaveAsVBA()
    {
        BOOL result;
        GetProperty(0xab, VT_BOOL, (void*)&result);
        return result;
    }
    void SetSaveAsVBA(BOOL propVal)
    {
        SetProperty(0xab, VT_BOOL, propVal);
    }
};
