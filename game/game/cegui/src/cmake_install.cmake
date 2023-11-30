# Install script for directory: D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/cegui")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/bin/CEGUIBase-0_d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/bin/CEGUIBase-0.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/bin/CEGUIBase-0.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/bin/CEGUIBase-0.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0_Static_d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0_Static.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0_Static.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/lib/CEGUIBase-0_Static.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cegui-0/CEGUI" TYPE FILE FILES
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Affector.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Animation.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/AnimationInstance.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/AnimationManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Animation_xmlHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Base.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/BasicImage.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/BasicRenderedStringParser.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/BidiVisualMapping.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/BoundSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/CEGUI.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/CentredRenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ChainedXMLHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Clipboard.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Colour.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ColourRect.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/CompositeResourceProvider.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Config_xmlHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/CoordConverter.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/DataContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/DefaultLogger.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/DefaultRenderedStringParser.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/DefaultResourceProvider.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/DynamicModule.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Element.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Event.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/EventArgs.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/EventSet.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Exceptions.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FactoryModule.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FactoryRegisterer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Font.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FontGlyph.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FontManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Font_xmlHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FormattedRenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ForwardRefs.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FreeFunctionSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FreeTypeFont.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FribidiVisualMapping.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FunctorCopySlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FunctorPointerSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FunctorReferenceBinder.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/FunctorReferenceSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/GUIContext.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/GUILayout_xmlHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/GeometryBuffer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/GlobalEventSet.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/IconvStringTranscoder.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Image.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ImageCodec.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ImageFactory.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ImageManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/InjectedInputReceiver.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/InputEvent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Interpolator.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/IteratorBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/JustifiedRenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/KeyFrame.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/LeftAlignedRenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/LinkedEvent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Logger.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemberFunctionSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemoryAllocatedObject.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemoryAllocation.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemoryOgreAllocator.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemorySTLWrapper.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MemoryStdAllocator.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MinibidiVisualMapping.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MinizipResourceProvider.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/MouseCursor.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/NamedElement.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/NamedXMLResourceManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/PCRERegexMatcher.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/PixmapFont.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Property.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/PropertyHelper.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/PropertySet.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Quaternion.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Rect.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RefCounted.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RegexMatcher.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderEffect.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderEffectFactory.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderEffectManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderQueue.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderTarget.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringImageComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringParser.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringTextComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringWidgetComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderedStringWordWrapper.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Renderer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderingContext.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderingSurface.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RenderingWindow.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ResourceProvider.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/RightAlignedRenderedString.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Scheme.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/SchemeManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Scheme_xmlHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/ScriptModule.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/SimpleTimer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Singleton.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Size.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/SlotFunctorBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/String.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/StringTranscoder.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/SubscriberSlot.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/System.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/SystemKeys.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TextUtils.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Texture.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TextureTarget.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplInterpolators.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplProperty.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWRFactoryRegisterer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWindowFactory.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWindowFactoryRegisterer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWindowProperty.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWindowRendererFactory.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TplWindowRendererProperty.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/TypedProperty.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/UDim.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Vector.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Vertex.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Win32StringTranscoder.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/Window.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/WindowFactory.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/WindowFactoryManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/WindowManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/WindowRenderer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/WindowRendererManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/XMLAttributes.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/XMLHandler.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/XMLParser.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/XMLSerializer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/cegui/src/../include/CEGUI/Config.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/cegui/src/../include/CEGUI/ModuleConfig.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/build/cegui/src/../include/CEGUI/Version.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cegui-0/CEGUI/widgets" TYPE FILE FILES
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/All.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ButtonBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ClippedContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ComboDropList.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Combobox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/DefaultWindow.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/DragContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Editbox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/FrameWindow.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/GridLayoutContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/GroupBox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/HorizontalLayoutContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ItemEntry.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ItemListBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ItemListbox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/LayoutCell.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/LayoutContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ListHeader.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ListHeaderSegment.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Listbox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ListboxItem.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ListboxTextItem.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/MenuBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/MenuItem.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Menubar.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/MultiColumnList.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/MultiLineEditbox.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/PopupMenu.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ProgressBar.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/PushButton.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/RadioButton.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ScrollablePane.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Scrollbar.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ScrolledContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ScrolledItemListBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/SequentialLayoutContainer.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Slider.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Spinner.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/TabButton.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/TabControl.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Thumb.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Titlebar.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/ToggleButton.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Tooltip.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/Tree.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/TreeItem.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/widgets/VerticalLayoutContainer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cegui-0/CEGUI/falagard" TYPE FILE FILES
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/ComponentBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/Dimensions.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/Enums.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/EventAction.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/EventLinkDefinition.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/FalagardPropertyBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/FormattingSetting.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/FrameComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/ImageryComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/ImagerySection.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/LayerSpecification.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/NamedArea.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/NamedDefinitionCollator.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/PropertyDefinition.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/PropertyDefinitionBase.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/PropertyInitialiser.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/PropertyLinkDefinition.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/SectionSpecification.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/StateImagery.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/TextComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/WidgetComponent.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/WidgetLookFeel.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/WidgetLookManager.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/XMLEnumHelper.h"
    "D:/RPT/Projects/dll/CEGUI/cegui-0.8.7/cegui-0.8.7/cegui/src/../include/CEGUI/falagard/XMLHandler.h"
    )
endif()

