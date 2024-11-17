#include "pch.h"
#include "framework.h"
#include "WirujaceKola.h"
#include "WirujaceKolaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CWirujaceKolaDlg::CWirujaceKolaDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_WIRUJACEKOLA_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    for (int i = 0; i < 4; ++i)
    {
        m_threads[i] = nullptr;
        m_threadRunning[i] = false;
        m_angles[i] = 0;
    }
}

void CWirujaceKolaDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWirujaceKolaDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CWirujaceKolaDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CWirujaceKolaDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CWirujaceKolaDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CWirujaceKolaDlg::OnBnClickedButton4)
    ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CWirujaceKolaDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    SetTimer(1, 50, NULL);
    return TRUE;
}

void CWirujaceKolaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CWirujaceKolaDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
        for (int i = 0; i < 4; ++i)
        {
            CStatic* pStatic = reinterpret_cast<CStatic*>(GetDlgItem(IDC_STATIC1 + i));
            if (pStatic != nullptr)
            {
                CRect rect;
                pStatic->GetClientRect(&rect);
                CDC* pDC = pStatic->GetDC();
                DrawCircle(pDC, rect, m_angles[i], RGB(0, 255, 0));
                pStatic->ReleaseDC(pDC);
            }
        }
    }
}

HCURSOR CWirujaceKolaDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CWirujaceKolaDlg::DrawCircle(CDC* pDC, CRect rect, int angle, COLORREF color)
{
    // Środek okręgu
    CPoint center = rect.CenterPoint();

    // Obliczamy promień, biorąc pod uwagę wielkość kontrolki
    int maxRadius = min(rect.Width(), rect.Height()) / 2 - 10; // Promień ograniczony przez kontrolkę

    // Obliczenie kąta w radianach
    double rad = angle * 3.14159 / 180;

    // Obliczanie pozycji (x, y) koła po obrocie
    int x = static_cast<int>(center.x + maxRadius * cos(rad));
    int y = static_cast<int>(center.y + maxRadius * sin(rad));

    // Ograniczenie współrzędnych x, y, aby koło nie wyszło poza kontrolkę
    x = max(rect.left + 10, min(x, rect.right - 10));
    y = max(rect.top + 10, min(y, rect.bottom - 10));

    // Rysowanie koła w obliczonej pozycji
    CPen pen(PS_SOLID, 2, color);  // Ustawienie koloru obwódki
    CBrush brush(color);            // Ustawienie koloru wypełnienia
    CBrush* oldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(CRect(x - 10, y - 10, x + 10, y + 10)); // Rysowanie okręgu
    pDC->SelectObject(oldBrush);  // Przywrócenie poprzedniego pędzla
}






UINT CWirujaceKolaDlg::RotateCircle(LPVOID pParam)
{
    auto* pData = static_cast<ThreadData*>(pParam);
    auto* pDlg = pData->pDlg;
    int index = pData->index;
    while (pDlg->m_threadRunning[index])
    {
        pDlg->m_angles[index] = (pDlg->m_angles[index] + 10) % 360;
        CStatic* pStatic = reinterpret_cast<CStatic*>(pDlg->GetDlgItem(IDC_STATIC1 + index));
        if (pStatic != nullptr)
        {
            pStatic->Invalidate();
        }
        Sleep(50);
    }
    delete pData;
    return 0;
}

void CWirujaceKolaDlg::OnBnClickedButton1()
{
    int index = 0;
    if (m_threadRunning[index])
        m_threadRunning[index] = false;
    else
    {
        m_threadRunning[index] = true;
        auto* pData = new ThreadData{ this, index };
        m_threads[index] = AfxBeginThread(RotateCircle, pData);
    }
}

void CWirujaceKolaDlg::OnBnClickedButton2()
{
    int index = 1;
    if (m_threadRunning[index])
        m_threadRunning[index] = false;
    else
    {
        m_threadRunning[index] = true;
        auto* pData = new ThreadData{ this, index };
        m_threads[index] = AfxBeginThread(RotateCircle, pData);
    }
}

void CWirujaceKolaDlg::OnBnClickedButton3()
{
    int index = 2;
    if (m_threadRunning[index])
        m_threadRunning[index] = false;
    else
    {
        m_threadRunning[index] = true;
        auto* pData = new ThreadData{ this, index };
        m_threads[index] = AfxBeginThread(RotateCircle, pData);
    }
}

void CWirujaceKolaDlg::OnBnClickedButton4()
{
    int index = 3;
    if (m_threadRunning[index])
        m_threadRunning[index] = false;
    else
    {
        m_threadRunning[index] = true;
        auto* pData = new ThreadData{ this, index };
        m_threads[index] = AfxBeginThread(RotateCircle, pData);
    }
}

void CWirujaceKolaDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        CStatic* pStatic = reinterpret_cast<CStatic*>(GetDlgItem(IDC_STATIC_ALL));
        if (pStatic != nullptr)
        {
            CRect rect;
            pStatic->GetClientRect(&rect);
            CDC* pDC = pStatic->GetDC();

            // Wyczyść tło
            pDC->FillSolidRect(rect, RGB(255, 255, 255));

            // Wyśrodkowanie kół
            int centerX = rect.CenterPoint().x;
            int centerY = rect.CenterPoint().y;

            // Rysowanie każdego koła w wyśrodkowanej pozycji
            for (int i = 0; i < 4; ++i)
            {
                COLORREF color = RGB(0, 255, 0); // Zielony
                if (i == 1) color = RGB(255, 0, 0); // Czerwony
                else if (i == 2) color = RGB(0, 0, 255); // Niebieski
                else if (i == 3) color = RGB(255, 255, 0); // Żółty

                // Oblicz przesunięcie dla każdego koła
                CRect circleRect = rect;
                circleRect.DeflateRect(50, 50); // dla rozmiaru koła

                // Nowe pozycje - wyśrodkowane w pionie i poziomie
                int offsetX = (i % 2 == 0) ? -50 : 50; // Wyśrodkowanie w poziomie
                int offsetY = (i < 2) ? -50 : 50; // Wyśrodkowanie w pionie

                circleRect.OffsetRect(centerX + offsetX - circleRect.CenterPoint().x, centerY + offsetY - circleRect.CenterPoint().y);

                // Rysowanie koła
                DrawCircle(pDC, circleRect, m_angles[i], color);
            }

            pStatic->ReleaseDC(pDC);
        }
    }
    CDialogEx::OnTimer(nIDEvent);
}




