//转换
#include <Windows.h>
#include <Winhttp.h>
static PCHAR WideStrToMultiStr(PWCHAR WideStr)
{
	ULONG nBytes;
	PCHAR MultiStr;
	nBytes = WideCharToMultiByte(CP_ACP, 0, WideStr, -1, NULL, 0, NULL, NULL);
	if (nBytes == 0)
	{
		return NULL;
	}
	MultiStr =(PCHAR) malloc(nBytes);
	if (MultiStr == NULL)
	{
		return NULL;
	}
	nBytes = WideCharToMultiByte(CP_ACP, 0, WideStr, -1, MultiStr, nBytes, NULL, NULL);
	if (nBytes == 0)
	{
		free(MultiStr);
		return NULL;
	}
	return MultiStr;
}
	
	
WINHTTP_CURRENT_USER_IE_PROXY_CONFIG pProxyConfig;
pProxyConfig.fAutoDetect = TRUE;
bool bState = WinHttpGetIEProxyConfigForCurrentUser(&pProxyConfig);
if (bState)
{
	DWORD Err = GetLastError();
	qDebug() << "check the error :" << Err;
	qDebug() << "ProxyConfig.lpszProxy" << pProxyConfig.fAutoDetect << pProxyConfig.lpszAutoConfigUrl << pProxyConfig.lpszProxy<< pProxyConfig.lpszProxyBypass;

	if (pProxyConfig.lpszProxy != nullptr)
	{
		PCHAR charstr1 = WideStrToMultiStr(pProxyConfig.lpszProxy);
		PCHAR charstr2 = WideStrToMultiStr(pProxyConfig.lpszProxyBypass);
		QString str1 = QString::fromLocal8Bit(charstr1);
		QString str2 = QString::fromLocal8Bit(charstr2);
		qDebug() << "charstr1r :" << str1 << str2;
		QStringList httpProxyStrList = str1.split(';');
		qDebug() << "proxyAddr" << httpProxyStrList;
		foreach (QString httpProxyStr, httpProxyStrList) {
			if (httpProxyStr.startsWith("http=")) {
				QString httpStr = httpProxyStr.remove("http=");
				curl_easy_setopt(easy_handle, CURLOPT_PROXY, httpStr.toStdString().c_str()); //代理服务器地址//MyProxyConfig.lpszProxy /"127.0.0.1:11359"
				curl_easy_setopt(easy_handle, CURLOPT_PROXYTYPE, CURLPROXY_HTTP); //设置http的代理类型
				//curl_easy_setopt(easy_handle, CURLOPT_PROXYUSERPWD, "user:password");
			} else if (httpProxyStr.startsWith("https=")) {
				QString httpStr = httpProxyStr.remove("https=");
				//curl_easy_setopt(easy_handle, CURLOPT_PROXY, httpStr.toStdString().c_str()); //代理服务器地址//MyProxyConfig.lpszProxy /"127.0.0.1:11359"
				//curl_easy_setopt(easy_handle, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS); //设置http的代理类型
			}
		}
		curl_easy_setopt(easy_handle, CURLOPT_HTTPPROXYTUNNEL, 1L); //若是https请求，设置通道
	}
	//WinHttpGetProxyForUrl(url.toEncoded().toStdString().c_str());
}