#pragma once

// Forward declaration klasy
class CWirujaceKolaDlg;

// Struktura do przekazywania danych do wątku
struct ThreadData
{
    CWirujaceKolaDlg* pDlg; // Wskaźnik na okno dialogowe
    int index;              // Indeks koła
};

// okno dialogowe CWirujaceKolaDlg
class CWirujaceKolaDlg : public CDialogEx
{
    // Konstrukcja
public:
    CWirujaceKolaDlg(CWnd* pParent = nullptr); // Konstruktor standardowy

    // Dane okna dialogowego
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_WIRUJACEKOLA_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // obsługa DDX/DDV

    // Implementacja
protected:
    HICON m_hIcon;

    // Wątki i status ich działania
    CWinThread* m_threads[4];     // Tablica wskaźników do wątków
    bool m_threadRunning[4];      // Status wątków
    int m_angles[4];              // Kąty obrotu dla kół

    // Funkcje pomocnicze
    static UINT RotateCircle(LPVOID pParam); // Funkcja wątku
    void DrawCircle(CDC* pDC, CRect rect, int angle, COLORREF color); // Rysowanie koła

    // Obsługa zdarzeń
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnTimer(UINT_PTR nIDEvent); // Funkcja timera

    // Mapa komunikatów
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
};
