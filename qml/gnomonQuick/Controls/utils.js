

function urlToPath(urlString) {
    var s
    if (urlString.startsWith("file:///")) {
        var k = urlString.charAt(9) === ':' ? 8 : 7
        s = urlString.substring(k)
    } else {
        s = urlString
    }
    return decodeURIComponent(s);
}