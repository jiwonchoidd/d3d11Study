#include "KWrite.h"
HRESULT KWrite::CreateDeviceResources(IDXGISurface1* pSurface)
{
    HRESULT hr = S_OK;

    UINT dpi = GetDpiForWindow(g_hWnd);
    D2D1_RENDER_TARGET_PROPERTIES props;
    props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format= DXGI_FORMAT_UNKNOWN;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    props.dpiX = dpi;
    props.dpiY = dpi;
    props.usage= D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel= D2D1_FEATURE_LEVEL_DEFAULT;

    hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
        pSurface, 
        &props, 
        &m_pRT);
    if (FAILED(hr)) return hr;

    m_pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Yellow), 
        &m_pTextBrush);
    if (FAILED(hr)) return hr;
    return S_OK;
}
bool KWrite::Init()
{
    toastTime=0.0f;
    alpha = 0.5;
    HRESULT hr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    if (FAILED(hr)) return false;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory), 
        reinterpret_cast<IUnknown**>(&m_pdWriteFactory));
    if (FAILED(hr)) return false;
    
     hr = m_pdWriteFactory->CreateTextFormat(
            L"Showcard Gothic",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"ko-kr",//L"en-us",//L"ko-kr",
            &m_pTextFormat
        );
     hr = m_pdWriteFactory->CreateTextFormat(
         L"Showcard Gothic",
         NULL,
         DWRITE_FONT_WEIGHT_NORMAL,
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_NORMAL,
         25,
         L"ko-kr",//L"en-us",//L"ko-kr",
         &m_pTextFormatRightAlign
     );
     hr = m_pdWriteFactory->CreateTextFormat(
         L"°íµñ",
         NULL,
         DWRITE_FONT_WEIGHT_NORMAL,
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_NORMAL,
         50,
         L"ko-kr",//L"en-us",//L"ko-kr",
         &m_pTextFormat50
     );
     //ÁÂ¿ìÁ¤·Ä
     m_pTextFormatRightAlign->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
     //»óÇÏÁ¤·Ä
     m_pTextFormatRightAlign->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    if (FAILED(hr)) return false;
    return true;
}

bool KWrite::Frame()
{
    return false;
}

bool KWrite::Render()
{
    return false;
}

bool KWrite::DrawText(RECT rt, 
    const TCHAR* data, D2D1::ColorF color,
    IDWriteTextFormat* pTextFormat)
{
    if (m_pRT)
    {
        m_pRT->BeginDraw();
        m_pRT->SetTransform(D2D1::IdentityMatrix());
        D2D1_RECT_F rect = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
        m_pTextBrush->SetColor(color);
        if (pTextFormat == nullptr)
        {
            m_pRT->DrawText(data, wcslen(data),
                m_pTextFormat, rect, m_pTextBrush);
        }
        else
        {
            m_pRT->DrawText(data, wcslen(data),
                pTextFormat, rect, m_pTextBrush);
        }
        m_pRT->EndDraw();
    }
    return true;
}
bool KWrite::BlinkMessage(wstring text)
{
        RECT  rt = { 0, 0, g_rtClient.right, g_rtClient.bottom};
        if (alpha < 0.0f)
        {
            alpha = 0.5f;
        }
        if(alpha > 0.0f)
        {
        DrawText(rt, text.c_str(), D2D1::ColorF{1.0f,1.0f,1.0f,alpha}, m_pTextFormatRightAlign);
        }
        alpha -= 0.25f * g_fSecPerFrame;
        return true;
}
bool KWrite::Release()
{
    if (m_pd2dFactory) m_pd2dFactory->Release();
    if (m_pdWriteFactory) m_pdWriteFactory->Release();
    if (m_pRT) m_pRT->Release();
    if (m_pTextFormat) m_pTextFormat->Release();
    if (m_pTextFormat50) m_pTextFormat50->Release();
    if (m_pTextBrush) m_pTextBrush->Release();
    
    return false;
}
