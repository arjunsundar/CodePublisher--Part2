var commentsHidden = false;
var classHidden = false;
var functionHidden = false;

function togglevisibility(classname){
       var elements  = document.getElementsByClassName(classname);
       if (classname == "comments"){
       if(commentsHidden) commentsHidden = false;
        else commentsHidden = true;
        var i;
        for(i=0; i<elements.length;i++)

        elements[i].hidden=commentsHidden;
}
       if (classname == "function"){
       if(functionHidden) functionHidden = false;
        else functionHidden = true;
        var i;
        for(i=0; i<elements.length;i++)

        elements[i].hidden=functionHidden;
}
if (classname == "class"){
       if(classHidden) classHidden = false;
        else classHidden = true;
        var i;
        for(i=0; i<elements.length;i++)

        elements[i].hidden=classHidden;
}

}