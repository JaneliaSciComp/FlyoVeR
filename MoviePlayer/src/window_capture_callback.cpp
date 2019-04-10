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

#include <iostream>
#include <sstream>
#include <string.h>

#include <window_capture_callback.h>

void
addCallbackToViewer( osgViewer::ViewerBase& viewer, WindowCaptureCallback* callback, bool remove )
{    
	if (callback->getFramePosition()==WindowCaptureCallback::START_FRAME)
	{
		osgViewer::ViewerBase::Windows windows;
		viewer.getWindows(windows);
		for(osgViewer::ViewerBase::Windows::iterator itr = windows.begin();
		    itr != windows.end();
		    ++itr)
		{
			osgViewer::GraphicsWindow* window = *itr;
			osg::GraphicsContext::Cameras& cameras = window->getCameras();
			osg::Camera* firstCamera = 0;
			for(osg::GraphicsContext::Cameras::iterator cam_itr = cameras.begin();
			    cam_itr != cameras.end();
			    ++cam_itr)
			{
				if (firstCamera)
				{
					if ((*cam_itr)->getRenderOrder() < firstCamera->getRenderOrder())
					{
						firstCamera = (*cam_itr);
					}
					if ((*cam_itr)->getRenderOrder() == firstCamera->getRenderOrder() &&
					    (*cam_itr)->getRenderOrderNum() < firstCamera->getRenderOrderNum())
					{
						firstCamera = (*cam_itr);
					}
				}
				else
				{
					firstCamera = *cam_itr;
				}
			}

			if (firstCamera)
			{
				osg::notify(osg::NOTICE)<<"First camera "<<firstCamera<<std::endl;

				if ( remove )
					firstCamera->setInitialDrawCallback( 0 );
				else
					firstCamera->setInitialDrawCallback( callback );
			}
			else
			{
				osg::notify(osg::NOTICE)<<"No camera found"<<std::endl;
			}
		}
	}
	else
	{    
		osgViewer::ViewerBase::Windows windows;
		viewer.getWindows(windows);
		for(osgViewer::ViewerBase::Windows::iterator itr = windows.begin();
		    itr != windows.end();
		    ++itr)
		{
			osgViewer::GraphicsWindow* window = *itr;
			osg::GraphicsContext::Cameras& cameras = window->getCameras();
			osg::Camera* lastCamera = 0;
			for(osg::GraphicsContext::Cameras::iterator cam_itr = cameras.begin();
			    cam_itr != cameras.end();
			    ++cam_itr)
			{
				if (lastCamera)
				{
					if ((*cam_itr)->getRenderOrder() > lastCamera->getRenderOrder())
					{
						lastCamera = (*cam_itr);
					}
					if ((*cam_itr)->getRenderOrder() == lastCamera->getRenderOrder() &&
					    (*cam_itr)->getRenderOrderNum() >= lastCamera->getRenderOrderNum())
					{
						lastCamera = (*cam_itr);
					}
				}
				else
				{
					lastCamera = *cam_itr;
				}
			}

			if (lastCamera)
			{
				osg::notify(osg::NOTICE)<<"Last camera "<<lastCamera<<std::endl;

				if ( remove )
					lastCamera->setFinalDrawCallback( 0 );
				else
					lastCamera->setFinalDrawCallback( callback );
			}
			else
			{
				osg::notify(osg::NOTICE)<<"No camera found"<<std::endl;
			}
		}
	}
}

void
WindowCaptureCallback::ContextData::updateTimings( osg::Timer_t tick_start,
                                                   osg::Timer_t tick_afterReadPixels,
                                                   osg::Timer_t tick_afterMemCpy,
                                                   unsigned int dataSize )
{
	if (!_reportTimingFrequency) return;

	double timeForReadPixels = osg::Timer::instance()->delta_s(tick_start, tick_afterReadPixels);
	double timeForFullCopy = osg::Timer::instance()->delta_s(tick_start, tick_afterMemCpy);
	double timeForMemCpy = osg::Timer::instance()->delta_s(tick_afterReadPixels, tick_afterMemCpy);

	_timeForReadPixels += timeForReadPixels;
	_timeForFullCopy += timeForFullCopy;
	_timeForMemCpy += timeForMemCpy;
    
	++_numTimeValuesRecorded;
    
	if (_numTimeValuesRecorded==_reportTimingFrequency)
	{
		timeForReadPixels = _timeForReadPixels/double(_numTimeValuesRecorded);
		timeForFullCopy = _timeForFullCopy/double(_numTimeValuesRecorded);
		timeForMemCpy = _timeForMemCpy/double(_numTimeValuesRecorded);
        
		double averageFrameTime =  osg::Timer::instance()->delta_s(_previousFrameTick, tick_afterMemCpy)/double(_numTimeValuesRecorded);
		double fps = 1.0/averageFrameTime;    
		_previousFrameTick = tick_afterMemCpy;

		_timeForReadPixels = 0.0;
		_timeForFullCopy = 0.0;
		_timeForMemCpy = 0.0;

		_numTimeValuesRecorded = 0;

		double numMPixels = double(_width * _height) / 1000000.0;
		double numMb = double(dataSize) / (1024*1024);

		int prec = osg::notify(osg::NOTICE).precision(5);

		if (timeForMemCpy==0.0)
		{
			osg::notify(osg::NOTICE)<<"fps = "<<fps<<", full frame copy = "<<timeForFullCopy*1000.0f<<"ms rate = "<<numMPixels / timeForFullCopy<<" Mpixel/sec, copy speed = "<<numMb / timeForFullCopy<<" Mb/sec"<<std::endl;
		}
		else
		{
			osg::notify(osg::NOTICE)<<"fps = "<<fps<<", full frame copy = "<<timeForFullCopy*1000.0f<<"ms rate = "<<numMPixels / timeForFullCopy<<" Mpixel/sec, "<<numMb / timeForFullCopy<< " Mb/sec "<<
				"time for memcpy = "<<timeForMemCpy*1000.0<<"ms  memcpy speed = "<<numMb / timeForMemCpy<<" Mb/sec"<<std::endl;
		}
		osg::notify(osg::NOTICE).precision(prec);

	}

}

void
WindowCaptureCallback::ContextData::readPixels()
{
    // std::cout<<"readPixels("<<_fileName<<" image "<<_currentImageIndex<<" "<<_currentPboIndex<<std::endl;

    unsigned int nextImageIndex = (_currentImageIndex+1)%_imageBuffer.size();
    unsigned int nextPboIndex = _pboBuffer.empty() ? 0 : (_currentPboIndex+1)%_pboBuffer.size();

    int width=0, height=0;
    getSize(_gc, width, height);
    if (width!=_width || _height!=height)
    {
        std::cout<<"   Window resized "<<width<<", "<<height<<std::endl;
        _width = width;
        _height = height;
    }

    osg::Image* image = _imageBuffer[_currentImageIndex].get();

    osg::Timer_t tick_start = osg::Timer::instance()->tick();

#if 1
    image->readPixels(0,0,_width,_height,
                      _pixelFormat,_type);
#endif

    osg::Timer_t tick_afterReadPixels = osg::Timer::instance()->tick();

    updateTimings(tick_start, tick_afterReadPixels, tick_afterReadPixels, image->getTotalSizeInBytes());

    if (!_file_prefix.empty())
    {
	    char str[ 1024 ];
	    std::stringstream sstm;
	    sstm.setf( std::ios::fixed, std:: ios::floatfield );
	    sstm.precision( 4 );
	    sstm << _trace->time();
	    std::string ss = sstm.str();
	    ss.replace( ss.find("."), 1, "_" );
	    sprintf( str, "%s/%s_%d_%s_%05d.jpg", _record_directory.c_str(), _file_prefix.c_str(), _frame_index, ss.c_str(), _index++ );

        osgDB::writeImageFile( *image, str );
    }

    _currentImageIndex = nextImageIndex;
    _currentPboIndex = nextPboIndex;
}

void
WindowCaptureCallback::ContextData::singlePBO( osg::GLBufferObject::Extensions* ext )
{
	// std::cout<<"singelPBO(  "<<_fileName<<" image "<<_currentImageIndex<<" "<<_currentPboIndex<<std::endl;

	unsigned int nextImageIndex = (_currentImageIndex+1)%_imageBuffer.size();

	int width=0, height=0;
	getSize(_gc, width, height);
	if (width!=_width || _height!=height)
	{
		std::cout<<"   Window resized "<<width<<", "<<height<<std::endl;
		_width = width;
		_height = height;
	}

	GLuint& pbo = _pboBuffer[0];
    
	osg::Image* image = _imageBuffer[_currentImageIndex].get();
	if (image->s() != _width || 
	    image->t() != _height)
	{
		osg::notify(osg::NOTICE)<<"Allocating image "<<std::endl;
		image->allocateImage(_width, _height, 1, _pixelFormat, _type);
        
		if (pbo!=0)
		{
			osg::notify(osg::NOTICE)<<"deleting pbo "<<pbo<<std::endl;
			ext->glDeleteBuffers (1, &pbo);
			pbo = 0;
		}
	}
    
    
	if (pbo==0)
	{
		ext->glGenBuffers(1, &pbo);
		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo);
		ext->glBufferData(GL_PIXEL_PACK_BUFFER_ARB, image->getTotalSizeInBytes(), 0, GL_STREAM_READ);

		osg::notify(osg::NOTICE)<<"Generating pbo "<<pbo<<std::endl;
	}
	else
	{
		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo);
	}

	osg::Timer_t tick_start = osg::Timer::instance()->tick();

#if 1
	glReadPixels(0, 0, _width, _height, _pixelFormat, _type, 0);
#endif

	osg::Timer_t tick_afterReadPixels = osg::Timer::instance()->tick();

	GLubyte* src = (GLubyte*)ext->glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB,
	                                          GL_READ_ONLY_ARB);
	if(src)
	{
		memcpy(image->data(), src, image->getTotalSizeInBytes());
        
		ext->glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
	}

	ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

	osg::Timer_t tick_afterMemCpy = osg::Timer::instance()->tick();

	updateTimings(tick_start, tick_afterReadPixels, tick_afterMemCpy, image->getTotalSizeInBytes());

	if (!_file_prefix.empty())
    {
	    char str[ 1024 ];
	    std::stringstream sstm;
	    sstm.setf( std::ios::fixed, std:: ios::floatfield );
	    sstm.precision( 4 );
	    sstm << _trace->time();
	    std::string ss = sstm.str();
	    ss.replace( ss.find("."), 1, "_" );
	    sprintf( str, "%s/%s_%d_%s_%05d.jpg", _record_directory.c_str(), _file_prefix.c_str(), _frame_index, ss.c_str(), _index++ );

        osgDB::writeImageFile( *image, str );
    }


	_currentImageIndex = nextImageIndex;
}

void
WindowCaptureCallback::ContextData::multiPBO( osg::GLBufferObject::Extensions* ext )
{
	// std::cout<<"multiPBO(  "<<_fileName<<" image "<<_currentImageIndex<<" "<<_currentPboIndex<<std::endl;
	unsigned int nextImageIndex = (_currentImageIndex+1)%_imageBuffer.size();
	unsigned int nextPboIndex = (_currentPboIndex+1)%_pboBuffer.size();

	int width=0, height=0;
	getSize(_gc, width, height);
	if (width!=_width || _height!=height)
	{
		std::cout<<"   Window resized "<<width<<", "<<height<<std::endl;
		_width = width;
		_height = height;
	}

	GLuint& copy_pbo = _pboBuffer[_currentPboIndex];
	GLuint& read_pbo = _pboBuffer[nextPboIndex];
    
	osg::Image* image = _imageBuffer[_currentImageIndex].get();
	if (image->s() != _width || 
	    image->t() != _height)
	{
		osg::notify(osg::NOTICE)<<"Allocating image "<<std::endl;
		image->allocateImage(_width, _height, 1, _pixelFormat, _type);
        
		if (read_pbo!=0)
		{
			osg::notify(osg::NOTICE)<<"deleting pbo "<<read_pbo<<std::endl;
			ext->glDeleteBuffers (1, &read_pbo);
			read_pbo = 0;
		}

		if (copy_pbo!=0)
		{
			osg::notify(osg::NOTICE)<<"deleting pbo "<<copy_pbo<<std::endl;
			ext->glDeleteBuffers (1, &copy_pbo);
			copy_pbo = 0;
		}
	}
    
    
	bool doCopy = copy_pbo!=0;
	if (copy_pbo==0)
	{
		ext->glGenBuffers(1, &copy_pbo);
		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, copy_pbo);
		ext->glBufferData(GL_PIXEL_PACK_BUFFER_ARB, image->getTotalSizeInBytes(), 0, GL_STREAM_READ);

		osg::notify(osg::NOTICE)<<"Generating pbo "<<read_pbo<<std::endl;
	}

	if (read_pbo==0)
	{
		ext->glGenBuffers(1, &read_pbo);
		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, read_pbo);
		ext->glBufferData(GL_PIXEL_PACK_BUFFER_ARB, image->getTotalSizeInBytes(), 0, GL_STREAM_READ);

		osg::notify(osg::NOTICE)<<"Generating pbo "<<read_pbo<<std::endl;
	}
	else
	{
		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, read_pbo);
	}

	osg::Timer_t tick_start = osg::Timer::instance()->tick();

#if 1
	glReadPixels(0, 0, _width, _height, _pixelFormat, _type, 0);
#endif

	osg::Timer_t tick_afterReadPixels = osg::Timer::instance()->tick();

	if (doCopy)
	{

		ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, copy_pbo);

		GLubyte* src = (GLubyte*)ext->glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB,
		                                          GL_READ_ONLY_ARB);
		if(src)
		{
			memcpy(image->data(), src, image->getTotalSizeInBytes());
			ext->glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
		}

		if (!_file_prefix.empty())
		{
			char str[ 1024 ];
			std::stringstream sstm;
			sstm.setf( std::ios::fixed, std:: ios::floatfield );
			sstm.precision( 4 );
			sstm << _trace->time( _trace->time_index()-2);
			std::string ss = sstm.str();
			ss.replace( ss.find("."), 1, "_" );
			sprintf( str, "%s/%s_%d_%s_%05d.jpg", _record_directory.c_str(), _file_prefix.c_str(), _frame_index, ss.c_str(), _index++ );

			osgDB::writeImageFile( *image, str );
		}
	}
    
	ext->glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

	osg::Timer_t tick_afterMemCpy = osg::Timer::instance()->tick();
    
	updateTimings(tick_start, tick_afterReadPixels, tick_afterMemCpy, image->getTotalSizeInBytes());

	_currentImageIndex = nextImageIndex;
	_currentPboIndex = nextPboIndex;
}

