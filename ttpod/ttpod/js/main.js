$(document)
.ready(
	function() {
		var head_ = $("head");
		var style_ = $("<style></style>");
		var class_ = '#_cc_tips{position:fixed;right:16px;width:290px;bottom:32px;z-index:100}'
		+ '#_cc_tips p{position:relative;background:rgba(0,0,0,.9);box-shadow:0 1px 2px rgba(0,0,0,.5);border-radius:2px;margin:10px 0px;padding:7px 10px 7px 5px;z-index:100}'
		+ '#_cc_tips p div{color:#FFF;padding-left:5px;border-left:5px solid #C00;font-family:"Segoe UI",Arial,"Microsoft YaHei",Sans-serif;font-size:12px;font-weight:bold;}'
		+ '#_cc_tips p div span{display:block;text-align:left;line-height:20px;z-index:100}';
		style_.attr("type", "text/css");
		style_.append(document.createTextNode(class_));

		head_.append(style_);
		var div_ = $("<div id='_cc_tips'></div>");
		$("body").append(div_);
	});

var showTip = function(elapse) {

	var __addTips__ = function(args) {
		for (var _div_ = $("<div></div>"), i = 0; i < args.length; i++) {
			var _new_ = $("<span></span>");
			_new_.html(args[i]);
			_div_.append(_new_);
		}
		var _p_ = $("<p></p>");
		_p_.append(_div_);
		$("#_cc_tips").append(_p_);
		__hover__(_p_, elapse * 1000);
		__fadeIn__(_p_, elapse * 1000);
	};

	var __hover__ = function(element, elapse) {
		element.hover(function() {
			$(this).stop().fadeIn("fast");
		}, __fadeOut__(element, 1000));
		element.mouseover(function() {
			$(this).stop(true, false);
		});
		element.click(function() {
			$(this).stop().fadeOut("fast", function() {
				$(this).remove();
			});
		});
	}

	var __fadeIn__ = function(element, elapse) {
		element.css("opacity", 0);
		element.css("display", "none");
		element.animate({
			height : 'show'
		}, 1000, function() {
			$(this).css("display", "none");
			$(this).css("opacity", "");
		}).fadeIn(1000).delay(elapse).fadeOut(2000, function() {
			$(this).css("opacity", 0);
			$(this).css("display", "block");
		}).animate({
			height : 'hide'
		}, 1000, function() {
			$(this).remove();
		});
	};

	var __fadeOut__ = function(element, elapse) {
		return function() {
			element.css("opacity", "");
			element.css("display", "block");
			element.delay(elapse).fadeOut(2000, function() {
				$(this).css("opacity", 0);
				$(this).css("display", "block");
			}).animate({
				height : 'hide'
			}, 1000, function() {
				$(this).remove();
			})
		};
	}

	var worker = function() {
	}

	return function() {
		__addTips__(arguments);
	};
}

var self = 0;

$(document).ready(function(){
	showTip(10)("<h4>欢迎光临！</h4>",
							"<a href='#'><h3>【在线音乐下载系统】</h3></a>", "&nbsp;",
							"<h5 align='right'>———— 2015 &copy; bajdcc</h5>");
    // Declare a proxy to reference the hub.
    var notifications = $.connection.ttpodBroadcast;
    // Create a function that the hub can call to broadcast messages.
    notifications.client.playByNotified = function (type, title, url) {
    	if (self == 1) {
    		showTip(3)(new Date().toLocaleString(), "<b>广播成功</b>");
    		self = 0;
    		return;
    	}
    	showTip(5)(new Date().toLocaleString(), "接收到广播：" + type);
    	if (type == "audio") {
    		online_play_audio(url, title);
    	} else if (type == "video") {
    		online_play_video(url, title);
    	} else {
			showTip(5)(new Date().toLocaleString(), "<b color='red'>广播系统遇到错误</b>");
    	}
    };
    // Start the connection.
    $.connection.hub.start();
});

function online_play_video(url, head) {
	showTip(5)(new Date().toLocaleString(), "播放视频：" + head);
	$("#play_dlg").remove();
	$( "#dl_dlg" ).dialog("close");
	$("body").append('<div id="play_dlg"></div>');
	var surl = "ckplayer.htm?s=" + encodeURIComponent(url);
	var s = '<iframe src="' + surl + '" frameborder="0" border="0" allowtransparency="true" width="820" height="620" id="dialogFrame" scrolling="no"></iframe>';
	$("#play_dlg").html(s);
	$("#play_dlg").dialog({
        autoOpen: true,
        closeOnEscape: true,
        modal: true,
        resizable: false,
        width: 860,
        height: 640,
        title: head,
    });
    $("#play_dlg").css("overflow", "hidden");
    $("#play_dlg").css("width", "");
    $("#play_dlg").css("height", "");
    $( "#play_dlg" ).on( "dialogclose", function( event, ui ) {
    	$(this).dialog("destroy"); $(this).remove();
    } );
}

function online_play_audio(url, head) {
	showTip(5)(new Date().toLocaleString(), "播放音频：" + head);
	$("#play_dlg").remove();
	$( "#dl_dlg" ).dialog("close");
	$("body").append('<div id="play_dlg"></div>');
	var surl = "peckplayer.htm?s=" + encodeURIComponent(url);
	var s = '<iframe src="' + surl + '" frameborder="0" border="0" allowtransparency="true" width="300" height="40" id="dialogFrame" scrolling="no"></iframe>';
	$("#play_dlg").html(s);
	$("#play_dlg").dialog({
        autoOpen: true,
        closeOnEscape: true,
        modal: true,
        resizable: false,
        width: 300,
        height: 40,
        title: head,
    });
    $("#play_dlg").css("overflow", "hidden");
    $("#play_dlg").css("width", "");
    $("#play_dlg").css("height", "");
    $( "#play_dlg" ).on( "dialogclose", function( event, ui ) {
    	$(this).dialog("destroy"); $(this).remove();
    } );
}

function broadcast_audio(url, head) {
	self = 1;
	$.connection.ttpodBroadcast.server.BroadcastToPlay("audio", head, url);
}

function broadcast_video(url, head) {
	self = 1;
	$.connection.ttpodBroadcast.server.BroadcastToPlay("video", head, url);
}

function asyncdl_audio(url, head) {
	showTip(3)(new Date().toLocaleString(), "离线下载音频：" + head);
	$.post("/dl/post.php", { url: url, name: head })
    .success(function() { showTip(5)(new Date().toLocaleString(), "成功添加任务"); })
    .error(function() { showTip(5)(new Date().toLocaleString(), "出现错误"); });
}

function asyncdl_video(url, head) {
	showTip(3)(new Date().toLocaleString(), "离线下载视频：" + head);
	$.post("/dl/post.php", { url: url, name: head })
    .success(function() { showTip(5)(new Date().toLocaleString(), "成功添加任务"); })
    .error(function() { showTip(5)(new Date().toLocaleString(), "出现错误"); });
}

$(document).ready(function () {
    $("#search").bigAutocomplete({
        url: "ttpodService.svc/Suggest",
        callback: function () {
            enterSearch()
        }
    });

    $("#submit").click(function () {
        enterSearch();
    });

    // Polyfiller
    webshims.polyfill();
    // load all available polyfills
});

var globalData = null;
var kstr = null;
var allpage = null;
var retrys = null;

function downloadData(id) {
    var url_ = "ttpodService.svc/Download";
    if (globalData == null || (allpage != null && allpage >= id)) {
        $.ajax({
            type: "GET",
            url: url_,
            data: {
                keyword: kstr,
                page: id
            },
            dataType: "json",
            success: function (data) {                
                if (globalData == null) {
                    globalData = JSON.parse(data);
                    allpage = globalData.allPage;
                } else {
                    globalData.data = globalData.data.concat(JSON.parse(data).data);
                }
                
                if (globalData.count > 500) {
                    $("#show").html('<b>(' + id + ' / ' + allpage + ')</b>&emsp;数据量较大，请稍候');
                }
                id++;
                setTimeout(function () {
                   downloadData(id);
                }, 20);
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                if (retrys < 10) {
                    setTimeout(function () {
                        downloadData(id);
                    }, 50);
                    retrys++;
                } else {
                    $("#show").html('网络出错');
                }
            }
        });
    } else {
        $("#show").html('正在分析数据');
        setTimeout(function () {
            analysisData();
        }, 20);
    }
}

function analysisData() {
    var dataSet = [];
    $.each(globalData.data, function (k, v) {
        var song = [
            k + 1,
            v.song_name,
            v.singer_name,
            v.album_name,
            v.pick_count,
        ];
        dataSet.push(song);
    });

    $("#show").html('<table id="dltable" class="display" cellspacing="0"></table>');

    //init
    $('#dltable').dataTable({
        language: {
            url: "i18n/Chinese.txt"
        },
        jQueryUI: true,
        paging: true,
        autoWidth: true,
        data: dataSet,
        columns: [
            { title: "序号", searchable: false, type: "numeric", width: "50px" },
            { title: "歌曲" },
            { title: "歌手" },
            { title: "专辑" },
            { title: "下载量", searchable: false, type: "numeric", width: "80px" },
        ],
        aoColumnDefs : [{
            sDefaultContent : '----',
            aTargets : [ '_all' ]
        }],
    });

    //event
    $('#dltable tbody').on('click', 'tr', function () {
        $("body").append('<div id="dl_dlg"></div>');
        var workbrk = 30;
        var so = $('td', this).eq(1).text();
        so = so.length > workbrk ? so.substr(0, workbrk) + '...' : so;
        var au = $('td', this).eq(2).text();
        au = au.length > workbrk ? au.substr(0, workbrk) + '...' : au;
        var s = '<div class="dialog-info"><div class="dialog-logo"><a>ttpod</a></div><span class="dialog-ps">歌曲：</span><span class="dialog-song"><a>'
        + so
        + '</a></span>&emsp;<span>-</span>&emsp;<span class="dialog-author"><a>'
        + au
        + '</a></span></div>';
        var t = '<div class="dialog-download"><span class="dialog-ps">品质：</span><div class="rate"><div class="ul">';
        selected = parseInt($('td', this).eq(0).text());
        var i = selected - 1;
        if (i < 0 || i >= globalData.data.length) {
            return;
        } 
        if (globalData.data[i].audition_list != null && globalData.data[i].audition_list.length > 0) {
            $.each(globalData.data[i].audition_list, function (k, v) {
                var l = '<div class="low-rate"><div class="li clearfix fsborder" data-url="'
                + v.url
                + '" data-ext="'
                + v.format
                + '"><label>'
                + "<a href='#' onclick='asyncdl_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "." + v.format + "&quot;)'>离线</a>&emsp;"
				+ '<a href="' + v.url + '">下载</a>&emsp;'
				+ "<a href='#' onclick='online_play_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>播放</a>&emsp;"
				+ "<a href='#' onclick='broadcast_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>广播</a>&emsp;"
				+ '<span class="rate-title">'
                + v.type_description
                +'</span><span class="dialog-c9">'
                + v.duration
                + '&emsp;</span><span class="dialog-c9">'
                + v.size
                +' / '
                + v.bitrate
                +'kbps / '
                + v.format
                + '</span></label></div></div>';
                t += l;
            });
        }
        if (globalData.data[i].ll_list != null && globalData.data[i].ll_list.length > 0) {
            $.each(globalData.data[i].ll_list, function (k, v) {
                var l = '<div class="high-rate"><div class="li clearfix fsborder"><label>'
                + "<a href='#' onclick='asyncdl_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "." + v.format + "&quot;)'>离线</a>&emsp;"       
				+ '<a href="' + v.url + '">下载</a>&emsp;'
				+ "<a href='#' onclick='online_play_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>播放</a>&emsp;"
				+ "<a href='#' onclick='broadcast_audio(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>广播</a>&emsp;"
				+ '<span class="rate-title">'
                + v.type_description
                + '</span><span class="dialog-c9">'
                + v.duration
                + '&emsp;</span><span class="dialog-c9">'
                + v.size
                + ' / '
                + v.bitrate
                + 'kbps / '
                + v.format
                + '</span></label></div></div>';
                t += l;
            });
        }
        if (globalData.data[i].mv_list != null && globalData.data[i].mv_list.length > 0) {
            $.each(globalData.data[i].mv_list, function (k, v) {
                var l = '<div class="high-rate"><div class="li clearfix fsborder"><label>'
                + "<a href='#' onclick='asyncdl_video(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "." + v.format + "&quot;)'>离线</a>&emsp;"        
				+ '<a href="' + v.url + '">下载</a>&emsp;'
				+ "<a href='#' onclick='online_play_video(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>播放</a>&emsp;"
				+ "<a href='#' onclick='broadcast_video(&quot;" + v.url + "&quot;, &quot;" + so + ' - '+ au + "&quot;)'>广播</a>&emsp;"
				+ '<span class="rate-title">'
                + v.type_description
                + '</span><span class="dialog-c9">'
                + v.duration
                + '&emsp;</span><span class="dialog-c9">'
                + v.size
                + ' / '
                + v.bitrate
                + 'kbps / '
                + v.format
                + '</span></label></div></div>';
                t += l;
            });
        }
        s += t;
        $("#dl_dlg").html(s);
        $("#dl_dlg").dialog({
            autoOpen: true,
            closeOnEscape: true,
            modal: true,
            resizable: false,
            title: "下载界面",
            width: "70%",
            buttons: [ { text: "关闭", click: function() { $(this).dialog("close"); } } ],
        });
        $("#dl_dlg").focus();
        $( "#dl_dlg" ).on( "dialogclose", function( event, ui ) {
    	    $(this).dialog("destroy"); $(this).remove();
        } );
    });
}

function enterSearch() {
    globalData = null;
    retrys = 0;
    kstr = $("#search").val();
    $("#show").html('正在下载数据');
    downloadData(1);
}
