var bridge = null;
var editor = null;

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

function send(name, value) {
    //console.log("JS: Sending", name, JSON.stringify(value));
    bridge.receive(name, JSON.stringify(value));
}

function receive(name, value) {
    //console.log("JS: Receiving", name, JSON.parse(value));

    var data = JSON.parse(value);
    let model = editor.getModel()
    switch (name) {
      case "value":
          model.setValue(data);
          monaco.editor.removeAllMarkers("owner");
          break;
      case "language":
          monaco.editor.setModelLanguage(editor.getModel(), data);
          break;
      case "theme":
          monaco.editor.setTheme(data);
          //send("theme", monaco.editor._themeService._theme.themeName);
          break;
      case "markers":
          const markers = [];
          //value =  {'type': 'parsing', 'class': 'NameError', 'message': "name 'a' is not defined", 'line': '?'}

          var startCol = -1;
          //not working yet .. :(
          if(data.class === "NameError" && data.line === '?') {
                const word = data.message.replace("name '","").replace("' is not defined","")
                //console.log("WORD ", word);
                const re = new RegExp("(\\s|\\W|^)"+ word + "(\\s|\\W|$)")
                for (let i = 1; i < model.getLineCount() + 1; i++) {
                    startCol = model.getLineContent(i).search(re);
                    //console.log("line " , i , "  is there ? " , startCol);
                    if(startCol >= 0) {
                        data.line = i;
                        break;
                    }
                }
          }

          startCol = startCol < 1 ? 1 : startCol;

          let d_endLine = isNaN(data.line) ? model.getLineCount() : parseInt(data.line);
          markers.push({
                message: data.class + ": " + data.message,
                severity: data.class.includes('Error') ? monaco.MarkerSeverity.Error : monaco.MarkerSeverity.Warning,
                startLineNumber: isNaN(data.line) ? 1 : parseInt(data.line),
                startColumn: startCol,
                endLineNumber: d_endLine,
                endColumn: model.getLineLength(d_endLine) + 1,
           });

          monaco.editor.setModelMarkers(model , "owner", markers);
          break;
    default:
          break;
    }
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

new QWebChannel(qt.webChannelTransport, function (channel) {

    bridge = channel.objects.bridge;
    bridge.sendDataChanged.connect(receive);

    require.config({ paths: { 'vs': 'dev/vs' } });
    require(['vs/editor/editor.main'], function () {
        editor = monaco.editor.create(document.getElementById('container'), {
            fontFamily: "Fira Code",
              scrollBeyondLastLine: false,
            automaticLayout: true
        });

        document.getElementById("container").style.height = "100%";

        editor.onDidChangeModelContent((event) => {
            monaco.editor.removeAllMarkers("owner");
            send("value", editor.getModel().getValue())
        });
        editor.onDidChangeModelLanguage((event) => {
            send("language", event.newLanguage)
        })

        bridge.init();
    });
});

/* Local Variables:   */
/* js-indent-level: 4 */
/* End:               */
