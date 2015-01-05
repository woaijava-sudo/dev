function autoSumbit(){
    for(var i=0;i<$("zbSize").value;i++){
       var obj=$("pjxx"+i);
       var divs=obj.getElementsByTagName("input");	
       for(var j=0;j<divs.length;j++){
            if(divs[j].type=='radio'){
                var ii = Math.floor(Math.random()*2);
                divs[ii].checked=true;
            }
        }
    }

    for(var i=0;i<$("wjSize").value;i++){
    	var s ="td"+i;
    	var td=$(s);
    	var td_type=j$("#td"+i).attr("type");
    	if(td_type=="radio"){
    		var ipt=td.getElementsByTagName("input");
    		var ii = Math.floor(Math.random()*2);
    		ipt[ii].checked=true;
    	}
    }
}