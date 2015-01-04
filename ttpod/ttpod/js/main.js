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
    var url_ = "ttpodService.svc/Download?";
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
        ]
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
                var l = '<div class="low-rate"><div class="li clearfix" data-url="'
                + v.url
                + '" data-ext="'
                + v.format
                + '"><label><span class="rate-title">'
                + v.type_description
                +'</span><span class="dialog-c9">'
                +v.duration
                + '&emsp;</span><span class="dialog-c9">'
                +v.size
                +' / '
                +v.bitrate
                +'kbps / '
                +v.format
                + '</span></label></div></div>';
                t += l;
            });
        }
        if (globalData.data[i].ll_list != null && globalData.data[i].ll_list.length > 0) {
            $.each(globalData.data[i].ll_list, function (k, v) {
                var l = '<div class="high-rate"><div class="li clearfix" data-url="'
                + v.url
                + '" data-ext="'
                + v.format
                + '"><label><span class="rate-title">'
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
                var l = '<div class="high-rate"><div class="li clearfix" data-url="'
                + v.url
                + '" data-ext="'
                + v.format
                + '"><label><span class="rate-title">'
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
        $.each($(".li.clearfix"), function (k, v) {
            $(this).click(function (e) {
                e.preventDefault();
                var s = "ttpodService.svc/Redirect?url=" + encodeURIComponent($(this).attr('data-url'));
                window.open(s);
                //window.location.href = $(this).attr('data-url');
            });
        });
        $("#dl_dlg").dialog({
            autoOpen: true,
            closeOnEscape: true,
            modal: true,
            resizable: false,
            title: "下载界面",
            width: "70%",
            buttons: [ { text: "关闭", click: function() { $( this ).dialog( "close" ); } } ]
        });
        $("#dl_dlg").on("dialogclose", function (event, ui) {
            $(this).dialog("destroy");
            $(this).remove();
        });
    });
}

function enterSearch() {
    globalData = null;
    retrys = 0;
    kstr = $("#search").val();
    $("#show").html('正在下载数据');
    downloadData(1);
}