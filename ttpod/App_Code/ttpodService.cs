using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.ServiceModel;
using System.ServiceModel.Activation;
using System.ServiceModel.Web;
using System.Text;
using System.Web;

[ServiceContract(Namespace = "")]
[AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
public class ttpodProxyService
{
	// 要使用 HTTP GET，请添加 [WebGet] 特性。(默认 ResponseFormat 为 WebMessageFormat.Json)
	// 要创建返回 XML 的操作，
	//     请添加 [WebGet(ResponseFormat=WebMessageFormat.Xml)]，
	//     并在操作正文中包括以下行:
	//         WebOperationContext.Current.OutgoingResponse.ContentType = "text/xml";
	[OperationContract]
    [WebInvoke(Method = "GET", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json,
    UriTemplate = "Suggest?keyword={keyword}", BodyStyle = WebMessageBodyStyle.WrappedRequest)]
    public string DoSuggest(string keyword)
	{
        var ret = "{}";
        try
        {
            var wc = new WebClient();
            wc.Encoding = Encoding.UTF8;
            ret = wc.DownloadString(new Uri("http://so.ard.iyyin.com/suggest.do?q=" + keyword));
        }
        catch (Exception)
        {
        }
        return ret;
	}

    [OperationContract]
    [WebInvoke(Method = "GET", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json,
    UriTemplate = "Download?keyword={keyword}&page={page}", BodyStyle = WebMessageBodyStyle.WrappedRequest)]
    public string DoDownload(string keyword, int page)
    {
        var ret = "{}";
        try
        {
            var wc = new WebClient();
            wc.Encoding = Encoding.UTF8;
            ret = wc.DownloadString(new Uri(string.Format("http://so.ard.iyyin.com/v2/songs/search?size=200&q={0}&page={1}", keyword, page)));
        }
        catch (Exception)
        {
        }
        return ret;
    }

    [OperationContract]
    [WebInvoke(Method = "GET", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json,
    UriTemplate = "Redirect?url={url}", BodyStyle = WebMessageBodyStyle.WrappedRequest)]
    public void DoRedirect(string url)
    {
        var strurl = "#";
        try 
	    {	        
            strurl = HttpUtility.UrlDecode(url);
	    }
	    catch (Exception)
	    {
	    }

        var response = WebOperationContext.Current.OutgoingResponse;
        response.StatusCode = HttpStatusCode.MovedPermanently;
        response.Location = strurl;
    }
}
