#ifndef WINDOW_CAPTURE_CALLBACK_H
#define WINDOW_CAPTURE_CALLBACK_H

/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commercial and non commercial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osg/CoordinateSystemNode>

#include <osg/Switch>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <iostream>
#include <sstream>
#include <string.h>

#include <MotionCallback.h>

class WindowCaptureCallback;

void addCallbackToViewer( osgViewer::ViewerBase& viewer, WindowCaptureCallback* callback, bool remove = false );

class WindowCaptureCallback : public osg::Camera::DrawCallback
{
public:
    
	enum Mode
	{
		READ_PIXELS,
		SINGLE_PBO,
		DOUBLE_PBO,
		TRIPLE_PBO
	};
    
	enum FramePosition
	{
		START_FRAME,
		END_FRAME
	};
    
	struct ContextData : public osg::Referenced
	{
        
		ContextData( osg::GraphicsContext* gc, Mode mode, GLenum readBuffer, int index, std::string dir,
		             std::string file_prefix, CameraTraceUpdateCallback* ct ):
			_gc(gc),
			_mode(mode),
			_readBuffer(readBuffer),
			_pixelFormat(GL_RGBA),
			_type(GL_UNSIGNED_BYTE),
			_width(0),
			_height(0),
			_currentImageIndex(0),
			_currentPboIndex(0),
			_reportTimingFrequency(100),
			_numTimeValuesRecorded(0),
			_timeForReadPixels(0.0),
			_timeForFullCopy(0.0),
			_timeForMemCpy(0.0),
			_record_directory( dir ), 
			_file_prefix( file_prefix ),
			_frame_index( index ), 
			_index( 0 ),
			_trace( ct )

		{
			_previousFrameTick = osg::Timer::instance()->tick();

			if (gc->getTraits())
			{
				if (gc->getTraits()->alpha)
				{
					osg::notify(osg::NOTICE)<<"Select GL_BGRA read back format"<<std::endl;
					_pixelFormat = GL_BGRA;
				}
				else 
				{
					osg::notify(osg::NOTICE)<<"Select GL_BGR read back format"<<std::endl;
					_pixelFormat = GL_RGB; 
				}
			}
            
			getSize(gc, _width, _height);
                
			std::cout<<"Window size "<<_width<<", "<<_height<<std::endl;
            
			// single buffered image
			_imageBuffer.push_back(new osg::Image);
                
			// double buffer PBO.
			switch(_mode)
			{
				case(READ_PIXELS):
					osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with out PixelBufferObject."<<std::endl;
					break;
				case(SINGLE_PBO): 
					osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a single PixelBufferObject."<<std::endl;
					_pboBuffer.push_back(0); 
					break;
				case(DOUBLE_PBO): 
					osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a double buffer PixelBufferObject."<<std::endl;
					_pboBuffer.push_back(0); 
					_pboBuffer.push_back(0); 
					break;
				case(TRIPLE_PBO): 
					osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a triple buffer PixelBufferObject."<<std::endl;
					_pboBuffer.push_back(0); 
					_pboBuffer.push_back(0); 
					_pboBuffer.push_back(0); 
					break;
				default:
					break;                                
			}
		}
            
		void getSize(osg::GraphicsContext* gc, int& width, int& height)
		{
			if (gc->getTraits())
			{
				width = gc->getTraits()->width;
				height = gc->getTraits()->height;
			}
		}
            
		void updateTimings(osg::Timer_t tick_start,
		                   osg::Timer_t tick_afterReadPixels,
		                   osg::Timer_t tick_afterMemCpy,
		                   unsigned int dataSize);

		void read()
		{
			osg::GLBufferObject::Extensions* ext = osg::GLBufferObject::getExtensions(_gc->getState()->getContextID(),true);

			if (ext->isPBOSupported() && !_pboBuffer.empty())
			{
				if (_pboBuffer.size()==1)
				{
					singlePBO(ext);
				}
				else
				{
					multiPBO(ext);
				}
			}
			else
			{
				readPixels();
			}
		}
            
		void readPixels();

		void singlePBO(osg::GLBufferObject::Extensions* ext);

		void multiPBO(osg::GLBufferObject::Extensions* ext);
        
		typedef std::vector< osg::ref_ptr<osg::Image> >             ImageBuffer;
		typedef std::vector< GLuint > PBOBuffer;
        
		osg::GraphicsContext*   _gc;
		Mode                    _mode;
		GLenum                  _readBuffer;
            
		GLenum                  _pixelFormat;
		GLenum                  _type;
		int                     _width;
		int                     _height;
            
		unsigned int            _currentImageIndex;
		ImageBuffer             _imageBuffer;
            
		unsigned int            _currentPboIndex;
		PBOBuffer               _pboBuffer;

		unsigned int            _reportTimingFrequency;
		unsigned int            _numTimeValuesRecorded;
		double                  _timeForReadPixels;
		double                  _timeForFullCopy;
		double                  _timeForMemCpy;
		osg::Timer_t            _previousFrameTick;

		std::string _record_directory, _file_prefix;
		int _frame_index;
		mutable int _index;
		CameraTraceUpdateCallback* _trace;
	};
    
	WindowCaptureCallback( Mode mode, FramePosition position, GLenum readBuffer, std::string dir,
	                       std::string file_prefix, CameraTraceUpdateCallback* ct ):
		_mode(mode),
		_position(position),
		_readBuffer(readBuffer),
		_record_directory( dir ), 
		_file_prefix( file_prefix ),
		_trace( ct )
	{
	}

	FramePosition getFramePosition() const { return _position; }

	ContextData* createContextData( osg::GraphicsContext* gc, std::string dir,
	                                std::string file_prefix ) const
	{
		return new ContextData(gc, _mode, _readBuffer, _contextDataMap.size(), dir, file_prefix, _trace );
	}
	
	ContextData* getContextData(osg::GraphicsContext* gc) const
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		osg::ref_ptr<ContextData>& data = _contextDataMap[gc];
		if (!data) data = createContextData( gc, _record_directory, _file_prefix );
            
		return data.get();
	}

	virtual void operator () (osg::RenderInfo& renderInfo) const
	{
		glReadBuffer(_readBuffer);

		osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
		osg::ref_ptr<ContextData> cd = getContextData(gc);
		cd->read();
	}
        
	typedef std::map<osg::GraphicsContext*, osg::ref_ptr<ContextData> > ContextDataMap;

	Mode                        _mode;        
	FramePosition               _position;
	GLenum                      _readBuffer;
	mutable OpenThreads::Mutex  _mutex;
	mutable ContextDataMap      _contextDataMap;
	std::string _record_directory, _file_prefix;
	CameraTraceUpdateCallback* _trace;
};

#endif
