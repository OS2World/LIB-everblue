#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define STR(def)# def

#define STUBMSG "function \"%s()\" not compatible!\n"
#define STUBMSG2 "function \"%s()\" not implemented yet!\n"

#define STUB(func) 				\
int func (void) 				\
{						\
	static int firsttime = 0;		\
	if (!(firsttime++))			\
	fprintf(stderr,STUBMSG,STR(func));	\
	return 0;				\
}

#define WEAKSTUB(func) 				\
int func (void) 				\
{						\
	static int firsttime = 0;		\
	if (!(firsttime++))			\
	fprintf(stderr,STUBMSG2,STR(func));	\
	return 0;				\
}

/// never supported
// low-level extension functions
STUB(XESetWireToEvent)
STUB(XESetEventToWire)
STUB(XESetError)
STUB(XESetWireToError)
STUB(_XDefaultWireError)
STUB(_XEventToWire)
STUB(_XUnknownWireEvent)
STUB(_XWireToEvent)
STUB(_XSetLastRequestRead)
STUB(_XRead)
STUB(_XReply)
STUB(_XSend)
STUB(_XEatData)
STUB(_XReadPad)
STUB(_XDeqAsyncHandler)
STUB(_XGetAsyncData)
STUB(_XUnknownNativeEvent)
STUB(_XGetAsyncReply)

/// not known
WEAKSTUB(dlclose)
WEAKSTUB(dlopen)
WEAKSTUB(dlsym)
WEAKSTUB(SyncHandle)
WEAKSTUB(_XGetHostname)
WEAKSTUB(_XReverse_Bytes)
WEAKSTUB(_XScreenOfWindow)
WEAKSTUB(_xudc_utyderror)
WEAKSTUB(_xudc_utyerror)
WEAKSTUB(_xudc_utyerrno)
WEAKSTUB(XauDisposeAuth)
WEAKSTUB(XauFileName)
WEAKSTUB(XauGetBestAuthByAddr)
WEAKSTUB(XauReadAuth)
WEAKSTUB(XSetAuthorization)
WEAKSTUB(_XEventsQueued)
WEAKSTUB(_X11TransBytesReadable)
WEAKSTUB(_X11TransClose)
WEAKSTUB(_X11TransCloseForCloning)
WEAKSTUB(_X11TransConnect)
WEAKSTUB(_X11TransConnectDisplay)
WEAKSTUB(_X11TransConvertAddress)
WEAKSTUB(_X11TransDisconnect)
WEAKSTUB(_X11TransFreeConnInfo)
WEAKSTUB(_X11TransGetConnectionNumber)
WEAKSTUB(_X11TransGetHostname)
WEAKSTUB(_X11TransGetMyAddr)
WEAKSTUB(_X11TransGetPeerAddr)
WEAKSTUB(_X11TransIsLocal)
WEAKSTUB(_X11TransOpenCLTSClient)
WEAKSTUB(_X11TransOpenCOTSClient)
WEAKSTUB(_X11TransRead)
WEAKSTUB(_X11TransReadv)
WEAKSTUB(_X11TransSetOption)
WEAKSTUB(_X11TransSocketINETFuncs)
WEAKSTUB(_X11TransSocketTCPFuncs)
WEAKSTUB(_X11TransWrite)
WEAKSTUB(_X11TransWritev)
WEAKSTUB(_XAllocID)
WEAKSTUB(_XAllocIDs)
WEAKSTUB(_XAsyncErrorHandler)
WEAKSTUB(_XConnectDisplay)
WEAKSTUB(_XDeq)
WEAKSTUB(_XDisconnectDisplay)
WEAKSTUB(_XEnq)
WEAKSTUB(_XError)
WEAKSTUB(_XFreeAtomTable)
WEAKSTUB(_XFreeDisplayStructure)
WEAKSTUB(_XInitKeysymDB)
WEAKSTUB(_XIOError)
WEAKSTUB(_XkeyTable)
WEAKSTUB(_XPollfdCacheAdd)
WEAKSTUB(_XPollfdCacheDel)
WEAKSTUB(_XPollfdCacheInit)
WEAKSTUB(_XProcessWindowAttributes)
WEAKSTUB(_XReadEvents)
WEAKSTUB(_XRegisterInternalConnection)
WEAKSTUB(_XSendClientPrefix)
WEAKSTUB(_XSetClipRectangles)
WEAKSTUB(_XSyncFunction)
WEAKSTUB(_XUnregisterInternalConnection)
WEAKSTUB(_XUpdateAtomCache)
WEAKSTUB(_XUpdateGCCache)
WEAKSTUB(__XOS2RedirRoot1)
WEAKSTUB(_XDefaultError)
WEAKSTUB(_XDefaultIOError)
WEAKSTUB(_XcmsGetTableType0)
WEAKSTUB(_XcmsGetTableType1)
WEAKSTUB(_XcmsIntensityCmp)
WEAKSTUB(_XcmsIntensityInterpolation)
WEAKSTUB(_XcmsMatVec)
WEAKSTUB(_XcmsTableSearch)
WEAKSTUB(_XcmsValueCmp)
WEAKSTUB(_XcmsValueInterpolation)

/// possibly supported - where does it make sense?
STUB(_XAllocScratch)
STUB(_XAllocTemp)
STUB(_XFreeTemp)
STUB(XMaxRequestSize)
STUB(XExtendedMaxRequestSize)
STUB(XUngrabServer)
STUB(XGrabServer)
WEAKSTUB(XSynchronize)
WEAKSTUB(XAddConnectionWatch)
WEAKSTUB(XRemoveConnectionWatch)
WEAKSTUB(XNextRequest)
WEAKSTUB(XLastKnownRequestProcessed)
WEAKSTUB(XProcessInternalConnection)
WEAKSTUB(XInternalConnectionNumbers)
WEAKSTUB(XAddHost)
WEAKSTUB(XAddHosts)
WEAKSTUB(XListHosts)
WEAKSTUB(XRemoveHost)
WEAKSTUB(XRemoveHosts)
WEAKSTUB(XSetAccessControl)
WEAKSTUB(XDisableAccessControl)
WEAKSTUB(XEnableAccessControl)
WEAKSTUB(XFreeFontPath)
WEAKSTUB(XGetFontPath)
WEAKSTUB(XSetFontPath)

/// Draw
WEAKSTUB(XDrawImageString)
WEAKSTUB(XDrawImageString16)
WEAKSTUB(XDrawString16)
WEAKSTUB(XDrawRectangle)
WEAKSTUB(XDrawArc)
WEAKSTUB(XDrawArcs)
WEAKSTUB(XFillArcs)
WEAKSTUB(XFillRectangles)
WEAKSTUB(XDrawPoint)
WEAKSTUB(XDrawPoints)
WEAKSTUB(XDrawRectangles)
WEAKSTUB(XDrawText)
WEAKSTUB(XDrawText16)
WEAKSTUB(XCopyPlane)

/// Pixmap/Cursor
WEAKSTUB(XFreePixmap)
WEAKSTUB(XUndefineCursor)
WEAKSTUB(XCreatePixmapCursor)
WEAKSTUB(XCreateGlyphCursor)
WEAKSTUB(XRecolorCursor)
WEAKSTUB(XSetAfterFunction)
WEAKSTUB(XSetInputFocus)
WEAKSTUB(XWarpPointer)
WEAKSTUB(XFreeCursor)

/// Fonts
WEAKSTUB(XFreeFont)
WEAKSTUB(XFreeFontInfo)
WEAKSTUB(XFreeFontNames)
WEAKSTUB(XListFonts)
WEAKSTUB(XListFontsWithInfo)
WEAKSTUB(XQueryTextExtents)
WEAKSTUB(XQueryTextExtents16)
WEAKSTUB(XUnloadFont)
WEAKSTUB(XQueryFont)

/// Keyboard
WEAKSTUB(XGetKeyboardMapping)
WEAKSTUB(XChangeKeyboardMapping)
WEAKSTUB(XNewModifiermap)
WEAKSTUB(XInsertModifiermapEntry)
WEAKSTUB(XDeleteModifiermapEntry)
WEAKSTUB(XFreeModifiermap)
WEAKSTUB(XSetModifierMapping)
WEAKSTUB(XGetModifierMapping)
WEAKSTUB(XChangeKeyboardControl)
WEAKSTUB(XGetKeyboardControl)
WEAKSTUB(XBell)
WEAKSTUB(XQueryKeymap)
WEAKSTUB(XSetPointerMapping)
WEAKSTUB(XGetPointerMapping)
WEAKSTUB(XChangePointerControl)
WEAKSTUB(XGetPointerControl)
WEAKSTUB(XStringToKeysym)
WEAKSTUB(XKeysymToString)
WEAKSTUB(XKeysymToKeyCode)

/// Event
WEAKSTUB(XPeekIfEvent)
WEAKSTUB(XCheckWindowEvent)
WEAKSTUB(XMaskEvent)
WEAKSTUB(XCheckMaskEvent)
WEAKSTUB(XCheckTypedEvent)
WEAKSTUB(XCheckTypedWindowEvent)
WEAKSTUB(XSendEvent)
WEAKSTUB(XGetMotionEvents)
WEAKSTUB(XCheckIfEvent)
WEAKSTUB(XFilterEvent)
WEAKSTUB(XAllowEvents)
WEAKSTUB(XWindowEvent)

/// Window
WEAKSTUB(XUnmapWindow)
WEAKSTUB(XRaiseWindow)
WEAKSTUB(XMapRaised)
WEAKSTUB(XReconfigureWMWindow)
WEAKSTUB(XConfigureWindow)
WEAKSTUB(XChangeWindowAttributes)
WEAKSTUB(XMoveWindow)
WEAKSTUB(XReparentWindow)
WEAKSTUB(XResizeWindow)
WEAKSTUB(XRestackWindows)
WEAKSTUB(XUnmapSubwindows)
WEAKSTUB(XCirculateSubwindows)
WEAKSTUB(XCirculateSubwindowsDown)
WEAKSTUB(XCirculateSubwindowsUp)
WEAKSTUB(XDestroySubwindows)
WEAKSTUB(XSetWindowBorderPixmap)
WEAKSTUB(XSetWindowBorder)
WEAKSTUB(XSetWindowBackground)
WEAKSTUB(XSetWindowBackgroundPixmap)
WEAKSTUB(XSetWindowBorderWidth)

/// properties & selections
WEAKSTUB(XListProperties)
WEAKSTUB(XChangeProperty)
WEAKSTUB(XRotateWindowProperties)
WEAKSTUB(XDeleteProperty)
WEAKSTUB(XConvertSelection)
WEAKSTUB(XSetSelectionOwner)
WEAKSTUB(XGetSelectionOwner)

/// Grab
WEAKSTUB(XUngrabKeyboard)
WEAKSTUB(XGrabKey)
WEAKSTUB(XGrabButton)
WEAKSTUB(XUngrabKey)
WEAKSTUB(XUngrabButton)
WEAKSTUB(XGrabKeyboard)
WEAKSTUB(XUngrabPointer)
WEAKSTUB(XChangeActivePointerGrab)
WEAKSTUB(XGrabPointer)

/// GC
WEAKSTUB(XCopyGC)
WEAKSTUB(XFlushGC)
WEAKSTUB(XChangeGC)
WEAKSTUB(XFreeGC)
WEAKSTUB(XGContextFromGC)
WEAKSTUB(XSetFont)
WEAKSTUB(XSetRegion)
WEAKSTUB(XSetClipMask)
WEAKSTUB(XSetClipRectangles)
WEAKSTUB(XSetClipOrigin)
WEAKSTUB(XSetDashes)
WEAKSTUB(XSetState)
WEAKSTUB(XSetPlaneMask)
WEAKSTUB(XSetStipple)
WEAKSTUB(XSetTile)
WEAKSTUB(XSetTSOrigin)

/// image manipulation
WEAKSTUB(XGetImage)
WEAKSTUB(XPutImage)
WEAKSTUB(XAddPixel)
WEAKSTUB(XDestroyImage)
WEAKSTUB(XGetPixel)
WEAKSTUB(XPutPixel)
WEAKSTUB(XSubImage)
WEAKSTUB(XGetSubImage)

/// locking
WEAKSTUB(XLockDisplay)
WEAKSTUB(XUnlockDisplay)
WEAKSTUB(XInitThreads)

/// Color
WEAKSTUB(XLookupColor)
WEAKSTUB(XcmsAllocNamedColor)
WEAKSTUB(XcmsLookupColor)
WEAKSTUB(XAllocColorCells)
WEAKSTUB(XAllocColorPlanes)
WEAKSTUB(XFreeColors)
WEAKSTUB(XStoreNamedColor)

/// other
WEAKSTUB(XActivateScreenSaver)
WEAKSTUB(XForceScreenSaver)
WEAKSTUB(XGetScreenSaver)
WEAKSTUB(XResetScreenSaver)
WEAKSTUB(XSetScreenSaver)
WEAKSTUB(XKillClient)
WEAKSTUB(XSetCloseDownMode)
WEAKSTUB(XAddToSaveSet)
WEAKSTUB(XChangeSaveSet)
WEAKSTUB(XRemoveFromSaveSet)

/// XKB functions
WEAKSTUB(XkbAddDeviceLedInfo)
WEAKSTUB(XkbAddGeomColor)
WEAKSTUB(XkbAddGeomDoodad)
WEAKSTUB(XkbAddGeomKey)
WEAKSTUB(XkbAddGeomKeyAlias)
WEAKSTUB(XkbAddGeomOutline)
WEAKSTUB(XkbAddGeomOverlay)
WEAKSTUB(XkbAddGeomOverlayKey)
WEAKSTUB(XkbAddGeomOverlayRow)
WEAKSTUB(XkbAddGeomProperty)
WEAKSTUB(XkbAddGeomRow)
WEAKSTUB(XkbAddGeomSection)
WEAKSTUB(XkbAddGeomShape)
WEAKSTUB(XkbAddKeyType)
WEAKSTUB(XkbAllocClientMap)
WEAKSTUB(XkbAllocCompatMap)
WEAKSTUB(XkbAllocControls)
WEAKSTUB(XkbAllocDeviceInfo)
WEAKSTUB(XkbAllocGeomColors)
WEAKSTUB(XkbAllocGeomDoodads)
WEAKSTUB(XkbAllocGeometry)
WEAKSTUB(XkbAllocGeomKeyAliases)
WEAKSTUB(XkbAllocGeomKeys)
WEAKSTUB(XkbAllocGeomOutlines)
WEAKSTUB(XkbAllocGeomOverlayKeys)
WEAKSTUB(XkbAllocGeomOverlayRows)
WEAKSTUB(XkbAllocGeomOverlays)
WEAKSTUB(XkbAllocGeomPoints)
WEAKSTUB(XkbAllocGeomProps)
WEAKSTUB(XkbAllocGeomRows)
WEAKSTUB(XkbAllocGeomSectionDoodads)
WEAKSTUB(XkbAllocGeomSections)
WEAKSTUB(XkbAllocGeomShapes)
WEAKSTUB(XkbAllocIndicatorMaps)
WEAKSTUB(XkbAllocKeyboard)
WEAKSTUB(XkbAllocNames)
WEAKSTUB(XkbAllocServerMap)
WEAKSTUB(XkbApplyCompatMapToKey)
WEAKSTUB(XkbApplyVirtualModChanges)
WEAKSTUB(XkbBell)
WEAKSTUB(XkbBellEvent)
WEAKSTUB(XkbChangeDeviceInfo)
WEAKSTUB(XkbChangeEnabledControls)
WEAKSTUB(XkbChangeMap)
WEAKSTUB(XkbChangeNames)
WEAKSTUB(XkbChangeTypesOfKey)
WEAKSTUB(XkbComputeEffectiveMap)
WEAKSTUB(XkbComputeRowBounds)
WEAKSTUB(XkbComputeSectionBounds)
WEAKSTUB(XkbComputeShapeBounds)
WEAKSTUB(XkbComputeShapeTop)
WEAKSTUB(XkbCopyKeyType)
WEAKSTUB(XkbCopyKeyTypes)
WEAKSTUB(XkbDeviceBell)
WEAKSTUB(XkbDeviceBellEvent)
WEAKSTUB(XkbFindOverlayForKey)
WEAKSTUB(XkbForceBell)
WEAKSTUB(XkbForceDeviceBell)
WEAKSTUB(XkbFreeClientMap)
WEAKSTUB(XkbFreeCompatMap)
WEAKSTUB(XkbFreeComponentList)
WEAKSTUB(XkbFreeControls)
WEAKSTUB(XkbFreeDeviceInfo)
WEAKSTUB(XkbFreeGeomColors)
WEAKSTUB(XkbFreeGeomDoodads)
WEAKSTUB(XkbFreeGeometry)
WEAKSTUB(XkbFreeGeomKeyAliases)
WEAKSTUB(XkbFreeGeomKeys)
WEAKSTUB(XkbFreeGeomOutlines)
WEAKSTUB(XkbFreeGeomOverlayKeys)
WEAKSTUB(XkbFreeGeomOverlayRows)
WEAKSTUB(XkbFreeGeomOverlays)
WEAKSTUB(XkbFreeGeomPoints)
WEAKSTUB(XkbFreeGeomProperties)
WEAKSTUB(XkbFreeGeomRows)
WEAKSTUB(XkbFreeGeomSections)
WEAKSTUB(XkbFreeGeomShapes)
WEAKSTUB(XkbFreeIndicatorMaps)
WEAKSTUB(XkbFreeKeyboard)
WEAKSTUB(XkbFreeNames)
WEAKSTUB(XkbFreeServerMap)
WEAKSTUB(XkbGetAutoRepeatRate)
WEAKSTUB(XkbGetAutoResetControls)
WEAKSTUB(XkbGetCompatMap)
WEAKSTUB(XkbGetControls)
WEAKSTUB(XkbGetDetectableAutoRepeat)
WEAKSTUB(XkbGetDeviceButtonActions)
WEAKSTUB(XkbGetDeviceInfo)
WEAKSTUB(XkbGetDeviceInfoChanges)
WEAKSTUB(XkbGetDeviceLedInfo)
WEAKSTUB(XkbGetGeometry)
WEAKSTUB(XkbGetIndicatorMap)
WEAKSTUB(XkbGetIndicatorState)
WEAKSTUB(XkbGetKeyActions)
WEAKSTUB(XkbGetKeyBehaviors)
WEAKSTUB(XkbGetKeyboard)
WEAKSTUB(XkbGetKeyboardByName)
WEAKSTUB(XkbGetKeyExplicitComponents)
WEAKSTUB(XkbGetKeyModifierMap)
WEAKSTUB(XkbGetKeySyms)
WEAKSTUB(XkbGetKeyTypes)
WEAKSTUB(XkbGetKeyVirtualModMap)
WEAKSTUB(XkbGetMap)
WEAKSTUB(XkbGetMapChanges)
WEAKSTUB(XkbGetNamedDeviceIndicator)
WEAKSTUB(XkbGetNamedGeometry)
WEAKSTUB(XkbGetNamedIndicator)
WEAKSTUB(XkbGetNames)
WEAKSTUB(XkbGetPerClientControls)
WEAKSTUB(XkbGetState)
WEAKSTUB(XkbGetUpdatedMap)
WEAKSTUB(XkbGetVirtualMods)
WEAKSTUB(XkbGetXlibControls)
WEAKSTUB(XkbIgnoreExtension)
WEAKSTUB(XkbInitCanonicalKeyTypes)
WEAKSTUB(XkbKeyTypesForCoreSymbols)
WEAKSTUB(XkbLatchGroup)
WEAKSTUB(XkbLatchModifiers)
WEAKSTUB(XkbLibraryVersion)
WEAKSTUB(XkbListComponents)
WEAKSTUB(XkbLockGroup)
WEAKSTUB(XkbLockModifiers)
WEAKSTUB(XkbNoteControlsChanges)
WEAKSTUB(XkbNoteDeviceChanges)
WEAKSTUB(XkbNoteMapChanges)
WEAKSTUB(XkbNoteNameChanges)
WEAKSTUB(XkbOpenDisplay)
WEAKSTUB(XkbQueryExtension)
WEAKSTUB(XkbResizeDeviceButtonActions)
WEAKSTUB(XkbResizeKeyActions)
WEAKSTUB(XkbResizeKeySyms)
WEAKSTUB(XkbResizeKeyType)
WEAKSTUB(XkbSelectEventDetails)
WEAKSTUB(XkbSelectEvents)
WEAKSTUB(XkbSetAtomFuncs)
WEAKSTUB(XkbSetAutoRepeatRate)
WEAKSTUB(XkbSetAutoResetControls)
WEAKSTUB(XkbSetCompatMap)
WEAKSTUB(XkbSetControls)
WEAKSTUB(XkbSetDebuggingFlags)
WEAKSTUB(XkbSetDetectableAutoRepeat)
WEAKSTUB(XkbSetDeviceButtonActions)
WEAKSTUB(XkbSetDeviceInfo)
WEAKSTUB(XkbSetDeviceLedInfo)
WEAKSTUB(XkbSetGeometry)
WEAKSTUB(XkbSetIgnoreLockMods)
WEAKSTUB(XkbSetIndicatorMap)
WEAKSTUB(XkbSetMap)
WEAKSTUB(XkbSetNamedDeviceIndicator)
WEAKSTUB(XkbSetNamedIndicator)
WEAKSTUB(XkbSetNames)
WEAKSTUB(XkbSetPerClientControls)
WEAKSTUB(XkbSetServerInternalMods)
WEAKSTUB(XkbSetXlibControls)
WEAKSTUB(XkbUpdateActionVirtualMods)
WEAKSTUB(XkbUpdateKeyTypeVirtualMods)
WEAKSTUB(XkbUpdateMapFromCore)
WEAKSTUB(XkbUseExtension)
WEAKSTUB(XkbVirtualModsToReal)
WEAKSTUB(XkbXlibControlsImplemented)
WEAKSTUB(_XkbCopyFromReadBuffer)
WEAKSTUB(_XkbFreeReadBuffer)
WEAKSTUB(_XkbGetAtomNameFunc)
WEAKSTUB(_XkbGetCharset)
WEAKSTUB(_XkbGetConverters)
WEAKSTUB(_XkbGetReadBufferCountedString)
WEAKSTUB(_XkbGetReadBufferPtr)
WEAKSTUB(_XkbInitReadBuffer)
WEAKSTUB(_XkbInternAtomFunc)
WEAKSTUB(_XkbNoteCoreMapChanges)
WEAKSTUB(_XkbPeekAtReadBuffer)
WEAKSTUB(_XkbReadBufferCopyKeySyms)
WEAKSTUB(_XkbReadCopyKeySyms)
WEAKSTUB(_XkbSkipReadBufferData)
WEAKSTUB(_XkbWriteCopyKeySyms)

/// XIM functions
WEAKSTUB(_XDefaultOpenIM)
WEAKSTUB(_XimCbDispatch)
WEAKSTUB(_XimCheckCreateICValues)
WEAKSTUB(_XimCheckDataSize)
WEAKSTUB(_XimCheckICMode)
WEAKSTUB(_XimCheckIfLocalProcessing)
WEAKSTUB(_XimCheckIfThaiProcessing)
WEAKSTUB(_XimCheckIMMode)
WEAKSTUB(_XimCheckLocalInputStyle)
WEAKSTUB(_XimCommitCallback)
WEAKSTUB(_XimConnect)
WEAKSTUB(_Ximctstombs)
WEAKSTUB(_Ximctstowcs)
WEAKSTUB(_XimDecodeICATTRIBUTE)
WEAKSTUB(_XimDecodeIMATTRIBUTE)
WEAKSTUB(_XimDecodeLocalICAttr)
WEAKSTUB(_XimDecodeLocalIMAttr)
WEAKSTUB(_XimDestroyIMStructureList)
WEAKSTUB(_XimDispatchInit)
WEAKSTUB(_XimEncodeICATTRIBUTE)
WEAKSTUB(_XimEncodeIMATTRIBUTE)
WEAKSTUB(_XimEncodeLocalICAttr)
WEAKSTUB(_XimEncodeLocalIMAttr)
WEAKSTUB(_XimError)
WEAKSTUB(_XimErrorCallback)
WEAKSTUB(_XimExtension)
WEAKSTUB(_XimFilterWaitEvent)
WEAKSTUB(_XimFlush)
WEAKSTUB(_XimForwardEvent)
WEAKSTUB(_XimForwardEventCallback)
WEAKSTUB(_XimFreeCommitInfo)
WEAKSTUB(_XimFreeProtoIntrCallback)
WEAKSTUB(_XimFreeTransIntrCallback)
WEAKSTUB(_XimGetAttributeID)
WEAKSTUB(_XimGetCurrentICValues)
WEAKSTUB(_XimGetCurrentIMValues)
WEAKSTUB(_XimGetICValueData)
WEAKSTUB(_XimGetIMValueData)
WEAKSTUB(_XimGetResourceListRec)
WEAKSTUB(_XimGetResourceListRecByQuark)
WEAKSTUB(_XimGetWindowEventmask)
WEAKSTUB(_XimICOfXICID)
WEAKSTUB(_XimImSportRec)
WEAKSTUB(_XimInitialResourceInfo)
WEAKSTUB(_XimLocalCreateIC)
WEAKSTUB(_XimLocalFilter)
WEAKSTUB(_XimLocalGetICValues)
WEAKSTUB(_XimLocalGetIMValues)
WEAKSTUB(_XimLocalIMFree)
WEAKSTUB(_XimLocalMbLookupString)
WEAKSTUB(_XimLocalOpenIM)
WEAKSTUB(_XimLocalSetICValues)
WEAKSTUB(_XimLocalSetIMValues)
WEAKSTUB(_XimLocalWcLookupString)
WEAKSTUB(_XimLookupMBText)
WEAKSTUB(_XimLookupWCText)
WEAKSTUB(_XimMakeICAttrIDList)
WEAKSTUB(_XimMakeIMAttrIDList)
WEAKSTUB(_XimParseStringFile)
WEAKSTUB(_XimProcError)
WEAKSTUB(_XimProcSyncReply)
WEAKSTUB(_XimProtoCreateIC)
WEAKSTUB(_XimProtoEventToWire)
WEAKSTUB(_XimProtoIMFree)
WEAKSTUB(_XimProtoMbLookupString)
WEAKSTUB(_XimProtoOpenIM)
WEAKSTUB(_XimProtoWcLookupString)
WEAKSTUB(_XimProtoWireToEvent)
WEAKSTUB(_XimRead)
WEAKSTUB(_XimRegisterDispatcher)
WEAKSTUB(_XimRegisterFilter)
WEAKSTUB(_XimRegisterIMInstantiateCallback)
WEAKSTUB(_XimRegisterServerFilter)
WEAKSTUB(_XimRegisterTriggerKeysCallback)
WEAKSTUB(_XimRegProtoIntrCallback)
WEAKSTUB(_XimReregisterFilter)
WEAKSTUB(_XimResetIMInstantiateCallback)
WEAKSTUB(_XimRespSyncReply)
WEAKSTUB(_XimServerDestroy)
WEAKSTUB(_XimSetCurrentICValues)
WEAKSTUB(_XimSetCurrentIMValues)
WEAKSTUB(_XimSetEventMaskCallback)
WEAKSTUB(_XimSetHeader)
WEAKSTUB(_XimSetICDefaults)
WEAKSTUB(_XimSetICMode)
WEAKSTUB(_XimSetICResourceList)
WEAKSTUB(_XimSetICValueData)
WEAKSTUB(_XimSetIMMode)
WEAKSTUB(_XimSetIMResourceList)
WEAKSTUB(_XimSetIMValueData)
WEAKSTUB(_XimSetInnerICResourceList)
WEAKSTUB(_XimSetInnerIMResourceList)
WEAKSTUB(_XimSetLocalIMDefaults)
WEAKSTUB(_XimShutdown)
WEAKSTUB(_XimSync)
WEAKSTUB(_XimSyncCallback)
WEAKSTUB(_XimThaiCloseIM)
WEAKSTUB(_XimThaiCreateIC)
WEAKSTUB(_XimThaiFilter)
WEAKSTUB(_XimThaiIMFree)
WEAKSTUB(_XimThaiOpenIM)
WEAKSTUB(_XimTransCallDispatcher)
WEAKSTUB(_XimTransConf)
WEAKSTUB(_XimTransFilterWaitEvent)
WEAKSTUB(_XimTransFlush)
WEAKSTUB(_XimTransInternalConnection)
WEAKSTUB(_XimTransportRec)
WEAKSTUB(_XimTransRead)
WEAKSTUB(_XimTransRegisterDispatcher)
WEAKSTUB(_XimTransWrite)
WEAKSTUB(_XimTriggerNotify)
WEAKSTUB(_XimUnregisterFilter)
WEAKSTUB(_XimUnRegisterIMInstantiateCallback)
WEAKSTUB(_XimUnregisterServerFilter)
WEAKSTUB(_XimWrite)
WEAKSTUB(_XimXConf)
WEAKSTUB(_XimXTransBytesReadable)
WEAKSTUB(_XimXTransClose)
WEAKSTUB(_XimXTransCloseForCloning)
WEAKSTUB(_XimXTransConnect)
WEAKSTUB(_XimXTransDisconnect)
WEAKSTUB(_XimXTransFreeConnInfo)
WEAKSTUB(_XimXTransGetConnectionNumber)
WEAKSTUB(_XimXTransGetHostname)
WEAKSTUB(_XimXTransGetMyAddr)
WEAKSTUB(_XimXTransGetPeerAddr)
WEAKSTUB(_XimXTransIsLocal)
WEAKSTUB(_XimXTransOpenCLTSClient)
WEAKSTUB(_XimXTransOpenCOTSClient)
WEAKSTUB(_XimXTransRead)
WEAKSTUB(_XimXTransReadv)
WEAKSTUB(_XimXTransSetOption)
WEAKSTUB(_XimXTransSocketINETFuncs)
WEAKSTUB(_XimXTransSocketTCPFuncs)
WEAKSTUB(_XimXTransWrite)
WEAKSTUB(_XimXTransWritev)
WEAKSTUB(_XInitIM)
