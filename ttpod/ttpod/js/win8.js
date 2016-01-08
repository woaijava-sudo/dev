$(document)
    .ready(
        function () {
            start_yy();
            setInterval(function () {
                start_yy();
            }, 5000);
        });
var YY_Status = 0;
function start_yy() {
    $.getJSON("hitokoto", function (data) {
        yy(data);
    });
}
function yy(data) {
    $("#hito").html(data["hitokoto"] + "<br><div style='text-align: right;'>— " + data["source"] + "</div>");
    $(".hito").css("margin-top", ($(".hito").parent().parent().height() - $(".hito").height()) / 2 - 10);
    switch (YY_Status) {
        case 0:
            $(".MYTime").css("left", $(".MYTime").width() + "px");
            YY_Status++;
            break;
        case 1:
            $(".MYTime").css("left", "0px");
            YY_Status++;
            break;
        case 2:
            $(".MYTime").css("left", "-" + $(".MYTime").width() + "px");
            YY_Status++;
            break;
        case 3:
            $(".MYTime").css("left", "0px");
            YY_Status = 0;
            break;
    }
}