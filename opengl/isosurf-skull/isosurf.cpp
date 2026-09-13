/////////////////////////////////////////////////////////////////////////////
// Name:        isosurf.cpp
// Purpose:     wxGLCanvas demo program (OBJ loader version)
// Author:      Brian Paul (original gltk version), Wolfram Gloger
// Modified by: Julian Smart, Francesco Montorsi
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "wx/timer.h"
#include "wx/glcanvas.h"
#include "wx/math.h"
#include "wx/log.h"
#include "wx/cmdline.h"
#include "wx/wfstream.h"
#include "wx/txtstrm.h"
#include "wx/tokenzr.h"

#include <map>
#include <utility>
#include <cmath>

#include "isosurf.h"
#include "../../sample.xpm"


// global options which can be set through command-line options
GLboolean g_use_vertex_arrays = GL_FALSE;
GLboolean g_doubleBuffer = GL_TRUE;
GLboolean g_smooth = GL_TRUE;
GLboolean g_lighting = GL_TRUE;


//---------------------------------------------------------------------------
// MyApp
//---------------------------------------------------------------------------

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // Create the main frame window
    new MyFrame(nullptr, "wxWidgets OpenGL Isosurf Sample");

    return true;
}

void MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.AddSwitch("", "sb", "Do not use double buffering");
    parser.AddSwitch("", "db", "Use double buffering");
    parser.AddSwitch("", "va", "Use vertex arrays");

    wxApp::OnInitCmdLine(parser);
}

bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (parser.Found("sb"))
        g_doubleBuffer = GL_FALSE;
    else if (parser.Found("db"))
        g_doubleBuffer = GL_TRUE;

    if (parser.Found("va"))
        g_use_vertex_arrays = GL_TRUE;

    return wxApp::OnCmdLineParsed(parser);
}

//---------------------------------------------------------------------------
// MyFrame
//---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
                 const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style),
      m_canvas(nullptr)
{
    SetIcon(wxICON(sample));

    // Make a menubar
    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_EXIT, "E&xit");
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    // Make a TestGLCanvas
    // JACS
#ifdef __WXMSW__
    int *gl_attrib = nullptr;
#else
    int gl_attrib[20] =
        { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
        WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
        WX_GL_DOUBLEBUFFER,
        0 };
#endif

    if (!g_doubleBuffer)
    {
        wxLogWarning("Disabling double buffering");

#ifdef __WXGTK__
        gl_attrib[9] = 0;
#endif
        g_doubleBuffer = GL_FALSE;
    }

    m_canvas = new TestGLCanvas(this, wxID_ANY, gl_attrib);

    // Show the frame
    Show(true);
    Raise();

    m_canvas->InitGL();
    m_canvas->LoadOBJ("Skull/12140_Skull_v3_L2.obj");
}

MyFrame::~MyFrame()
{
    delete m_canvas;
}

// Intercept menu commands
void MyFrame::OnExit( wxCommandEvent& WXUNUSED(event) )
{
    // true is to force the frame to close
    Close(true);
}


//---------------------------------------------------------------------------
// TestGLCanvas
//---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
    EVT_SIZE(TestGLCanvas::OnSize)
    EVT_PAINT(TestGLCanvas::OnPaint)
    EVT_CHAR(TestGLCanvas::OnChar)
    EVT_MOUSE_EVENTS(TestGLCanvas::OnMouseEvent)
wxEND_EVENT_TABLE()

TestGLCanvas::TestGLCanvas(wxWindow *parent,
                           wxWindowID id,
                           int* gl_attrib)
    : wxGLCanvas(parent, id, gl_attrib)
{
    m_xrot = 0.0f;
    m_yrot = 0.0f;
    m_zoom = 1.0f;
    m_cameraDist = 3.0f;  // recalcularado no primeiro OnSize
    m_center[0] = m_center[1] = m_center[2] = 0.0f;
    m_radius = 1.0f;

    // Explicitly create a new rendering context instance for this canvas.
    m_glRC = new wxGLContext(this);

    // Achar o foco para teclado
    SetFocus();
}

TestGLCanvas::~TestGLCanvas()
{
    delete m_glRC;
}

//---------------------------------------------------------------------------
// OBJ loader
//---------------------------------------------------------------------------
bool TestGLCanvas::LoadOBJ(const wxString& filename)
{
    wxFFileInputStream file(filename);
    if (!file.IsOk())
    {
        wxLogError("Cannot open '%s'", filename);
        return false;
    }

    wxTextInputStream in(file);

    std::vector<GLfloat> positions; // from "v" lines
    std::vector<GLfloat> normals;   // from "vn" lines

    m_positions.clear();
    m_normals.clear();
    m_indices.clear();

    // map (v_index, vn_index) -> output vertex index
    std::map<std::pair<int,int>, GLuint> vertexMap;

    while (!file.Eof())
    {
        wxString line = in.ReadLine();
        line.Trim(true);
        line.Trim(false);

        if (line.IsEmpty() || line[0] == '#')
            continue;

        wxString rest;
        if (line.StartsWith("v ", &rest))
        {
            double x = 0, y = 0, z = 0;
            wxSscanf(rest, "%lf %lf %lf", &x, &y, &z);
            positions.push_back((GLfloat)x);
            positions.push_back((GLfloat)y);
            positions.push_back((GLfloat)z);
        }
        else if (line.StartsWith("vn ", &rest))
        {
            double x = 0, y = 0, z = 0;
            wxSscanf(rest, "%lf %lf %lf", &x, &y, &z);
            normals.push_back((GLfloat)x);
            normals.push_back((GLfloat)y);
            normals.push_back((GLfloat)z);
        }
        else if (line.StartsWith("f ", &rest))
        {
            // formats supported:
            //   f v1 v2 v3
            //   f v1/vt1 v2/vt2 v3/vt3
            //   f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            wxStringTokenizer tok(rest, " ");
            std::vector<GLuint> face;
            while (tok.HasMoreTokens())
            {
                wxString token = tok.GetNextToken();

                wxString vStr  = token;
                wxString vnStr;

                int firstSlash = token.Find('/');
                if (firstSlash != wxNOT_FOUND)
                {
                    vStr = token.Left(firstSlash);
                    wxString after = token.Mid(firstSlash + 1);
                    int secondSlash = after.Find('/');
                    if (secondSlash != wxNOT_FOUND)
                        vnStr = after.Mid(secondSlash + 1);
                }

                long vIdx = 0, vnIdx = 0;
                vStr.ToLong(&vIdx);
                if (!vnStr.IsEmpty()) vnStr.ToLong(&vnIdx);

                // OBJ indices are 1-based; negative means relative
                int vi = (vIdx > 0)
                    ? (int)vIdx - 1
                    : (int)(positions.size() / 3) + (int)vIdx;

                int ni = -1;
                if (!vnStr.IsEmpty())
                {
                    ni = (vnIdx > 0)
                        ? (int)vnIdx - 1
                        : (int)(normals.size() / 3) + (int)vnIdx;
                }

                if (vi < 0 || vi * 3 + 2 >= (int)positions.size())
                    continue;

                std::pair<int,int> key(vi, ni);
                auto it = vertexMap.find(key);

                GLuint outIdx;
                if (it == vertexMap.end())
                {
                    outIdx = (GLuint)(m_positions.size() / 3);

                    m_positions.push_back(positions[vi*3+0]);
                    m_positions.push_back(positions[vi*3+1]);
                    m_positions.push_back(positions[vi*3+2]);

                    if (ni >= 0 && ni * 3 + 2 < (int)normals.size())
                    {
                        m_normals.push_back(normals[ni*3+0]);
                        m_normals.push_back(normals[ni*3+1]);
                        m_normals.push_back(normals[ni*3+2]);
                    }
                    else
                    {
                        m_normals.push_back(0.0f);
                        m_normals.push_back(0.0f);
                        m_normals.push_back(1.0f);
                    }

                    vertexMap[key] = outIdx;
                }
                else
                {
                    outIdx = it->second;
                }

                face.push_back(outIdx);
            }

            // triangulate the polygon as a fan
            for (size_t i = 1; i + 1 < face.size(); ++i)
            {
                m_indices.push_back(face[0]);
                m_indices.push_back(face[i]);
                m_indices.push_back(face[i+1]);
            }
        }
    }

    if (m_positions.empty())
    {
        wxLogError("No vertices loaded from '%s'", filename);
        return false;
    }

    // Se algum vértice ficou sem normal (fallback), calcula normais
    // acumulando as normais das faces e normalizando.
    bool anyMissingNormal = false;
    for (size_t i = 0; i + 2 < m_normals.size(); i += 3)
    {
        if (m_normals[i] == 0.0f &&
            m_normals[i+1] == 0.0f &&
            m_normals[i+2] == 1.0f)
        {
            anyMissingNormal = true;
            break;
        }
    }

    if (anyMissingNormal)
    {
        std::vector<GLfloat> acc(m_positions.size(), 0.0f);
        for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
        {
            GLuint i0 = m_indices[i+0];
            GLuint i1 = m_indices[i+1];
            GLuint i2 = m_indices[i+2];

            GLfloat ax = m_positions[i1*3+0] - m_positions[i0*3+0];
            GLfloat ay = m_positions[i1*3+1] - m_positions[i0*3+1];
            GLfloat az = m_positions[i1*3+2] - m_positions[i0*3+2];

            GLfloat bx = m_positions[i2*3+0] - m_positions[i0*3+0];
            GLfloat by = m_positions[i2*3+1] - m_positions[i0*3+1];
            GLfloat bz = m_positions[i2*3+2] - m_positions[i0*3+2];

            GLfloat nx = ay*bz - az*by;
            GLfloat ny = az*bx - ax*bz;
            GLfloat nz = ax*by - ay*bx;

            acc[i0*3+0] += nx; acc[i0*3+1] += ny; acc[i0*3+2] += nz;
            acc[i1*3+0] += nx; acc[i1*3+1] += ny; acc[i1*3+2] += nz;
            acc[i2*3+0] += nx; acc[i2*3+1] += ny; acc[i2*3+2] += nz;
        }
        for (size_t v = 0; v < acc.size(); v += 3)
        {
            GLfloat len = std::sqrt(acc[v]*acc[v] + acc[v+1]*acc[v+1] + acc[v+2]*acc[v+2]);
            if (len > 1e-8f)
            {
                m_normals[v+0] = acc[v+0] / len;
                m_normals[v+1] = acc[v+1] / len;
                m_normals[v+2] = acc[v+2] / len;
            }
        }
    }

    // bounding sphere
    GLfloat minv[3] = {  1e30f,  1e30f,  1e30f };
    GLfloat maxv[3] = { -1e30f, -1e30f, -1e30f };
    for (size_t i = 0; i < m_positions.size(); i += 3)
    {
        for (int k = 0; k < 3; ++k)
        {
            GLfloat v = m_positions[i+k];
            if (v < minv[k]) minv[k] = v;
            if (v > maxv[k]) maxv[k] = v;
        }
    }
    for (int k = 0; k < 3; ++k)
        m_center[k] = 0.5f * (minv[k] + maxv[k]);

    GLfloat dx = maxv[0] - minv[0];
    GLfloat dy = maxv[1] - minv[1];
    GLfloat dz = maxv[2] - minv[2];
    m_radius = 0.5f * std::sqrt(dx*dx + dy*dy + dz*dz);
    if (m_radius <= 1e-6f)
        m_radius = 1.0f;

    wxLogMessage("Loaded %zu vertices, %zu triangles from '%s'",
                 m_positions.size() / 3,
                 m_indices.size() / 3,
                 filename);

    // Recalcula projeção / distância de câmera com o novo raio
    {
        wxSizeEvent evt(GetSize());
        OnSize(evt);
    }

    PostSizeEventToParent();
    Refresh(false);
    return true;
}

//---------------------------------------------------------------------------
// Drawing
//---------------------------------------------------------------------------
void TestGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // OnPaint handlers must always create a wxPaintDC.
    wxPaintDC dc(this);

    SetCurrent(*m_glRC);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // câmera na distância calculada (que já leva em conta o zoom)
    glTranslatef(0.0f, 0.0f, -m_cameraDist);

    // rotação do usuário
    glRotatef(m_xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yrot, 0.0f, 1.0f, 0.0f);

    // centraliza o modelo na origem
    glTranslatef(-m_center[0], -m_center[1], -m_center[2]);

    if (g_use_vertex_arrays)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m_positions.data());
        glNormalPointer(GL_FLOAT, 0, m_normals.data());
        glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(),
                       GL_UNSIGNED_INT, m_indices.data());
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else
    {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i + 2 < m_indices.size(); i += 3)
        {
            for (int k = 0; k < 3; ++k)
            {
                GLuint idx = m_indices[i + k];
                glNormal3fv(&m_normals[idx * 3]);
                glVertex3fv(&m_positions[idx * 3]);
            }
        }
        glEnd();
    }

    glFlush();

    SwapBuffers();
}

void TestGLCanvas::OnSize(wxSizeEvent& event)
{
    if ( !IsShownOnScreen() )
        return;

    SetCurrent(*m_glRC);

    const wxSize size = event.GetSize() * GetContentScaleFactor();
    if (size.x <= 0 || size.y <= 0)
        return;

    glViewport(0, 0, size.x, size.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspect = (GLfloat)size.x / (GLfloat)size.y;

    // FOV vertical fixo (45°)
    const GLfloat fovY = 45.0f * (GLfloat)M_PI / 180.0f;
    GLfloat top = std::tan(fovY * 0.5f);

    // margem pequena (5%) → modelo ocupa quase toda a tela
    GLfloat margin = 1.05f;
    GLfloat distV = (m_radius * margin) / top;
    GLfloat distH = (m_radius * margin) / (top * aspect);

    // distância da câmera considerando o zoom atual
    m_cameraDist = (distV > distH ? distV : distH) / m_zoom;

    GLfloat nearP = m_cameraDist * 0.05f;
    GLfloat farP  = m_cameraDist * 10.0f;

    glFrustum(-top*aspect*nearP, top*aspect*nearP,
              -top*nearP,       top*nearP,
              nearP, farP);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TestGLCanvas::OnChar(wxKeyEvent& event)
{
    bool needRefresh = true;

    switch( event.GetKeyCode() )
    {
    case WXK_ESCAPE:
        wxTheApp->ExitMainLoop();
        return;

    case WXK_LEFT:
        m_yrot -= 15;
        break;

    case WXK_RIGHT:
        m_yrot += 15;
        break;

    case WXK_UP:
        m_xrot += 15;
        break;

    case WXK_DOWN:
        m_xrot -= 15;
        break;

    case '+': case '=':
        m_zoom *= 1.1f;
        break;

    case '-': case '_':
        m_zoom /= 1.1f;
        if (m_zoom < 0.05f) m_zoom = 0.05f;
        break;

    case 's': case 'S':
        g_smooth = !g_smooth;
        glShadeModel(g_smooth ? GL_SMOOTH : GL_FLAT);
        break;

    case 'l': case 'L':
        g_lighting = !g_lighting;
        if (g_lighting) glEnable(GL_LIGHTING);
        else            glDisable(GL_LIGHTING);
        break;

    case 'r': case 'R':
        // reset
        m_xrot = 0.0f;
        m_yrot = 0.0f;
        m_zoom = 1.0f;
        break;

    default:
        needRefresh = false;
        event.Skip();
        return;
    }

    if (needRefresh)
    {
        // Zoom altera a distância da câmera → reaplica projeção
        wxSizeEvent evt(GetSize());
        OnSize(evt);
        Refresh(false);
    }
}

void TestGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
    static int dragging = 0;
    static float last_x = 0.0f, last_y = 0.0f;

    // Deixa o wx processar o resto (foco, etc.)
    event.Skip();

    // ---- Zoom com a roda do mouse ----
    if (event.GetWheelRotation() != 0)
    {
        int rot = event.GetWheelRotation();
        float factor = 1.0f + (rot / 120.0f) * 0.1f; // ~10% por "click" da roda
        if (factor < 0.5f) factor = 0.5f;

        m_zoom *= factor;
        if (m_zoom < 0.05f) m_zoom = 0.05f;
        if (m_zoom > 100.0f) m_zoom = 100.0f;

        // zoom afeta m_cameraDist → recalcula projeção
        wxSizeEvent evt(GetSize());
        OnSize(evt);
        Refresh(false);
        return;
    }

    // ---- Rotação com botão esquerdo ----
    if (event.LeftIsDown())
    {
        if (!dragging)
        {
            dragging = 1;
        }
        else
        {
            m_yrot += event.GetX() - last_x;
            m_xrot += event.GetY() - last_y;
            Refresh(false);
        }
        last_x = (float)event.GetX();
        last_y = (float)event.GetY();
    }
    else
    {
        dragging = 0;
    }
}

void TestGLCanvas::InitMaterials()
{
    static const GLfloat ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    static const GLfloat diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    static const GLfloat position0[4] = {0.0f, 0.0f, 1.0f, 0.0f};
    static const GLfloat position1[4] = {0.0f, 0.0f, -1.0f, 0.0f};
    static const GLfloat front_mat_shininess[1] = {60.0f};
    static const GLfloat front_mat_specular[4] = {0.3f, 0.3f, 0.3f, 1.0f};
    static const GLfloat front_mat_diffuse[4] = {0.85f, 0.80f, 0.75f, 1.0f};
    static const GLfloat lmodel_ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    static const GLfloat lmodel_twoside[1] = {GL_FALSE};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glEnable(GL_LIGHT1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

    if (g_lighting)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, front_mat_diffuse);
}

void TestGLCanvas::InitGL()
{
    // Make the new context current (activate it for use) with this canvas.
    SetCurrent(*m_glRC);

    glClearColor(0.1f, 0.1f, 0.15f, 0.0f);

    glShadeModel(g_smooth ? GL_SMOOTH : GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    InitMaterials();

    glEnable(GL_NORMALIZE);
    // Se o modelo aparecer "oco"/estranho, descomente:
    // glEnable(GL_CULL_FACE);

    // Projeção default (recalculada no OnSize assim que soubermos o raio)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
