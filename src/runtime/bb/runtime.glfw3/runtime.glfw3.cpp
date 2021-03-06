
#include "../../stdutil/stdutil.h"
#include "runtime.glfw3.h"
#include <bb/pixmap/pixmap.h>
#include <bb/event/event.h>

#include <FreeImage.h>

#include <map>
using namespace std;

map<GLFWwindow*,GLFW3Runtime*> runtimes;

BBRuntime *bbCreateRuntime(){
  if( !glfwInit() ) return 0;

  glfwWindowHint( GLFW_FOCUSED,1 );
  glfwWindowHint( GLFW_RESIZABLE,0 );

  GLFWwindow* window=glfwCreateWindow( 1,1,"",NULL,NULL );
  if( !window ){
    glfwTerminate();
    return 0;
  }

  return d_new GLFW3Runtime( window );
}

GLFW3Runtime::GLFW3Runtime( GLFWwindow *wnd ):wnd(wnd),graphics(0){
  bbContextDriver=this;
  bbSceneDriver=this;

  runtimes.insert( pair<GLFWwindow*,GLFW3Runtime*>( wnd,this ) );

  glfwSetCursorPosCallback( wnd,_onMouseMove );
	glfwSetKeyCallback( wnd,_onKeyChange );
  glfwSetFramebufferSizeCallback( wnd,_onResize );
  glfwSetWindowCloseCallback( wnd,_onClose );
}

GLFW3Runtime::~GLFW3Runtime(){
  glfwTerminate();
}

void GLFW3Runtime::_onClose( GLFWwindow *w ){
  // RTEX( 0 );
  exit(1); // FIXME: need to get exception handling working...
}

int GLFW3Runtime::numGraphicsDrivers(){
  return 0;
}

void GLFW3Runtime::graphicsDriverInfo( int driver,std::string *name,int *c ){
}

int GLFW3Runtime::numGraphicsModes( int driver ){
  return 0;
}

void GLFW3Runtime::graphicsModeInfo( int driver,int mode,int *w,int *h,int *d,int *c ){
}

void GLFW3Runtime::windowedModeInfo( int *c ){
}

class GLFW3DefaultCanvas : public GLB2DDefaultCanvas{
protected:
  GLFWwindow *wnd;
public:
  GLFW3DefaultCanvas( GLFWwindow *wnd ):wnd(wnd){
  }

  // int getWidth()const{
  //   // int width,height;
  //   // glfwGetWindowSize( wnd,&width,&height );
  //   // return width;
  // }
  //
  // int getHeight()const{
  //   // int width,height;
  //   // glfwGetWindowSize( wnd,&width,&height );
  //   // return height;
  // }

  void getViewport( int *x,int *y,int *w,int *h )const{
    *x=0;*y=0;*w=getWidth();*h=getHeight();
  }
};

class GLFW3Graphics : public B2DGraphics{
protected:
  GLFWwindow *wnd;

public:
  GLFW3Graphics( GLFWwindow *wnd ):wnd(wnd){
    front_canvas=d_new GLFW3DefaultCanvas( wnd );
    back_canvas=d_new GLFW3DefaultCanvas( wnd );

		int w,h;
		glfwGetFramebufferSize( wnd,&w,&h );
		resize( w,h );
  }

  ~GLFW3Graphics(){
    if( front_canvas ) delete front_canvas;
    if( back_canvas ) delete back_canvas;
    front_canvas=back_canvas=0;
  }

  void resize( int w,int h ){
    ((GLB2DCanvas*)front_canvas)->resize( w,h );
    ((GLB2DCanvas*)back_canvas)->resize( w,h );
  }

  // graphics
  void backup(){}
  bool restore(){ return true; }

  //MANIPULATORS
  void vwait(){}

  //SPECIAL!
  void copy( BBCanvas *dest,int dx,int dy,int dw,int dh,BBCanvas *src,int sx,int sy,int sw,int sh ){}

  //NEW! Gamma control!
  void setGamma( int r,int g,int b,float dr,float dg,float db ){}
  void getGamma( int r,int g,int b,float *dr,float *dg,float *db ){}
  void updateGamma( bool calibrate ){}

  //ACCESSORS
  int getWidth()const{ return front_canvas->getWidth(); }
  int getHeight()const{ return front_canvas->getHeight(); }
  int getDepth()const{ return 0; }
  int getScanLine()const{ return 0; }
  int getAvailVidmem()const{ return 0; }
  int getTotalVidmem()const{ return 0; }

  BBFont *getDefaultFont()const{ return 0; }

  //OBJECTS
  BBCanvas *createCanvas( int width,int height,int flags ){
    BBCanvas *canvas=d_new GLB2DTextureCanvas( 0 );
    canvas_set.insert( canvas );
    return canvas;
  }

  BBCanvas *loadCanvas( const std::string &file,int flags ){
    cout<<"loadCanvas: "<<file<<endl;

    BBPixmap *pixmap=bbLoadPixmap( file );
    if( !pixmap ) return 0;

    BBCanvas *canvas=d_new GLB2DTextureCanvas( pixmap );
    canvas_set.insert( canvas );
    delete pixmap;
    return canvas;
  }

  // b2dgraphics
  BBMovie *openMovie( const std::string &file,int flags ){ return 0; }
  BBMovie *verifyMovie( BBMovie *movie ){ return 0; }
  void closeMovie( BBMovie *movie ){}

  BBFont *loadFont( const std::string &font,int height,int flags ){ return 0; }
  BBFont *verifyFont( BBFont *font ){ return 0; }
  void freeFont( BBFont *font ){}
};

BBGraphics *GLFW3Runtime::openGraphics( int w,int h,int d,int driver,int flags ){
  if( graphics ) return 0;

  if( (graphics=d_new GLFW3Graphics( wnd )) ){
    glfwSetWindowSize( wnd,w,h );
    glfwShowWindow( wnd );
    glfwMakeContextCurrent( wnd );
    return graphics;
  }
  return 0;
}

void GLFW3Runtime::closeGraphics( BBGraphics *g ){
  if( graphics!=g || !g ) return;
  delete graphics;graphics=0;
  glfwHideWindow( wnd );
}

bool GLFW3Runtime::graphicsLost(){
  return true;
}

void GLFW3Runtime::flip( bool vwait ){
  glfwSwapBuffers( wnd );
}

void GLFW3Runtime::asyncStop(){
}

void GLFW3Runtime::asyncRun(){
}

void GLFW3Runtime::asyncEnd(){
}

bool GLFW3Runtime::idle(){
  glfwPollEvents();
  return true;
}

void *GLFW3Runtime::window(){
  return wnd;
}

void GLFW3Runtime::moveMouse( int x,int y ){
}

void GLFW3Runtime::setPointerVisible( bool vis ){
}

void GLFW3Runtime::_onMouseMove( GLFWwindow *w,double x,double y ){
  int sw,sh;
  glfwGetWindowSize( w,&sw,&sh );
  BBEvent ev( BBEVENT_MOUSEMOVE,0,(x/sw)*bbGraphicsWidth(),(y/sh)*bbGraphicsHeight() );
  bbOnEvent.run( &ev );
}

extern unsigned short GLFW_SCANCODE_MAP[316];

void GLFW3Runtime::_onKeyChange( GLFWwindow *w,int key,int scancode,int action,int mods ){
	cout<<"key: "<<key<<endl;
	key=GLFW_SCANCODE_MAP[key-32];
	if( !key ) return;

	BBEvent ev;
	switch( action ){
	case GLFW_PRESS:
		ev=BBEvent( BBEVENT_KEYDOWN,key );
		break;
	case GLFW_RELEASE:
		ev=BBEvent( BBEVENT_KEYUP,key );
		break;
	}
	if( ev.id ) bbOnEvent.run( &ev );
}

void GLFW3Runtime::_onResize( GLFWwindow *w,int width,int height ){
  GLFW3Runtime *rt=runtimes.at( w );
  rt->resize( width,height );
}

void GLFW3Runtime::resize( int width,int height ){
  if( graphics ) graphics->resize( width,height );
}

BBMODULE_CREATE( runtime_glfw3 ){
  return true;
}

BBMODULE_DESTROY( runtime_glfw3 ){
  return true;
}
