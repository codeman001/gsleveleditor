/*
** gluos.h - operating system dependencies for GLU
**
** $Header: //depot/main/gfx/lib/glu/include/gluos.h#4 $
*/

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOIME
//#include <windows.h>

/* Disable warnings */
#pragma warning(disable : 4101)
#pragma warning(disable : 4244)
#pragma warning(disable : 4761)

#define GLAPI
#define WINGDIAPI

#else

/* Disable Microsoft-specific keywords */
#define GLAPI
#define WINGDIAPI

#endif

#define GLenum		int
#define GLboolean	char
#define GLfloat		float
#define GLdouble	double

#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009

/****           Tesselation constants           ****/
#define GLU_TESS_MAX_COORD              1.0e150

/* Tessellation properties */
#define GLU_TESS_WINDING_RULE           100140
#define GLU_TESS_BOUNDARY_ONLY          100141
#define GLU_TESS_TOLERANCE              100142

/* Winding rules */
#define GLU_TESS_WINDING_ODD            100130
#define GLU_TESS_WINDING_NONZERO        100131
#define GLU_TESS_WINDING_POSITIVE       100132
#define GLU_TESS_WINDING_NEGATIVE       100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO    100134

/* Errors */
#define GLU_INVALID_ENUM                100900
#define GLU_INVALID_VALUE               100901
#define GLU_OUT_OF_MEMORY               100902
#define GLU_INCOMPATIBLE_GL_VERSION     100903

/* TessCallback */
#define GLU_TESS_BEGIN          100100  /* void (CALLBACK*)(GLenum    type)  */
#define GLU_TESS_VERTEX         100101  /* void (CALLBACK*)(void      *data) */
#define GLU_TESS_END            100102  /* void (CALLBACK*)(void)            */
#define GLU_TESS_ERROR          100103  /* void (CALLBACK*)(GLenum    errno) */
#define GLU_TESS_EDGE_FLAG      100104  /* void (CALLBACK*)(GLboolean boundaryEdge)  */
#define GLU_TESS_COMBINE        100105  /* void (CALLBACK*)(GLdouble  coords[3],
                                                            void      *data[4],
                                                            GLfloat   weight[4],
                                                            void      **dataOut)     */
#define GLU_TESS_BEGIN_DATA     100106  /* void (CALLBACK*)(GLenum    type,  
                                                            void      *polygon_data) */
#define GLU_TESS_VERTEX_DATA    100107  /* void (CALLBACK*)(void      *data, 
                                                            void      *polygon_data) */
#define GLU_TESS_END_DATA       100108  /* void (CALLBACK*)(void      *polygon_data) */
#define GLU_TESS_ERROR_DATA     100109  /* void (CALLBACK*)(GLenum    errno, 
                                                            void      *polygon_data) */
#define GLU_TESS_EDGE_FLAG_DATA 100110  /* void (CALLBACK*)(GLboolean boundaryEdge,
                                                            void      *polygon_data) */
#define GLU_TESS_COMBINE_DATA   100111  /* void (CALLBACK*)(GLdouble  coords[3],
                                                            void      *data[4],
                                                            GLfloat   weight[4],
                                                            void      **dataOut,
                                                            void      *polygon_data) */

/* TessError */
#define GLU_TESS_ERROR1     100151
#define GLU_TESS_ERROR2     100152
#define GLU_TESS_ERROR3     100153
#define GLU_TESS_ERROR4     100154
#define GLU_TESS_ERROR5     100155
#define GLU_TESS_ERROR6     100156
#define GLU_TESS_ERROR7     100157
#define GLU_TESS_ERROR8     100158

#define GLU_TESS_MISSING_BEGIN_POLYGON  GLU_TESS_ERROR1
#define GLU_TESS_MISSING_BEGIN_CONTOUR  GLU_TESS_ERROR2
#define GLU_TESS_MISSING_END_POLYGON    GLU_TESS_ERROR3
#define GLU_TESS_MISSING_END_CONTOUR    GLU_TESS_ERROR4
#define GLU_TESS_COORD_TOO_LARGE        GLU_TESS_ERROR5
#define GLU_TESS_NEED_COMBINE_CALLBACK  GLU_TESS_ERROR6

struct GLUtesselator;

GLUtesselator* GLAPI gluNewTess();
void GLAPI gluDeleteTess( GLUtesselator *tess );

void GLAPI gluTessBeginPolygon( GLUtesselator *tess, void *data );
void GLAPI gluTessEndPolygon( GLUtesselator *tess );

void GLAPI gluTessBeginContour( GLUtesselator *tess );
void GLAPI gluTessEndContour( GLUtesselator *tess );

void GLAPI gluTessVertex( GLUtesselator *tess, GLdouble coords[3], void *data );
void GLAPI gluTessCallback( GLUtesselator *tess, GLenum which, void (GLAPI *fn)());
void GLAPI gluTessProperty( GLUtesselator *tess, GLenum which, GLdouble value );
