GRAPHVIZ_COLORMAP = {
    "black": "base05",
    "white": "base00",
    "midnightblue": "base0D",
    "darkgreen": "base0B",
    "orange": "base09",
    "red": "base08",
    "#9a32cd": "base0E",
    "#8b1a1a": "base08",
    "#bfbfbf": "base03",
    "#404040": "base04",
    "#daedff": "base0C",
    "#4ea9ff": "base0D"
};

function replaceColors(element) {
    const attrib = element.attributes;
    if ('stroke' in attrib && attrib['stroke'].nodeValue != '' && attrib['stroke'].nodeValue != 'none' && attrib['stroke'].nodeValue in GRAPHVIZ_COLORMAP) {
        element.classList.add(GRAPHVIZ_COLORMAP[element.getAttribute("stroke")])
        element.removeAttribute('stroke');
    }
    if ('fill' in attrib && attrib['fill'].nodeValue != '' && attrib['fill'].nodeValue != 'none' && attrib['fill'].nodeValue in GRAPHVIZ_COLORMAP) {
        element.classList.add(`${GRAPHVIZ_COLORMAP[element.getAttribute("fill")]}Fill`)
        element.removeAttribute('fill');
    }
}

window.onload = function () {
    document.getElementById("MSearchResults").onload = function () {
        var iframe = document.getElementById("MSearchResults").contentWindow.document;
        iframe.documentElement.classList.add(document.documentElement.classList[0]);
        var link = iframe.createElement('link');
        link.type = "text/css";
        link.rel = "stylesheet";
        link.href = "../override-search.css";
        iframe.head.appendChild(link);
    }

    var iframes = document.getElementsByTagName("iframe");
    for (var i = 0; i < iframes.length; ++i) {
        if (!iframes[i].src.endsWith(".svg"))
            continue;
        var svg = iframes[i].contentWindow.document.getElementsByTagName("svg")[0];
        svg.classList.add(document.documentElement.classList[0]);

        var link = document.createElement('link');
        link.type = "text/css";
        link.rel = "stylesheet";
        link.href = "../override-svg.css";
        svg.prepend(link);

        svg.querySelectorAll("path").forEach(replaceColors);
        svg.querySelectorAll("polygon").forEach(replaceColors);
    }
}