function init_chart_data(renderTo, title, yAxis_title) {
    var chart = new Highcharts.Chart({
        chart: { renderTo: renderTo },
        title: { text: title },
        series: [{
            showInLegend: false,
            data: []
        }],
        plotOptions: {
            line: {
                animation: false,
                dataLabels: { enabled: true }
            },
            series: { color: '#059e8a' }
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { second: '%H:%M:%S' }
        },
        yAxis: {
            title: { text: yAxis_title }
        },
        credits: { enabled: false }
    });
    return chart;
}

function update_temperature_chart() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            var x = data["timestamp"]*1000,
                y = data["value"];
            if (chartT.series[0].data.length > 96) {
                chartT.series[0].addPoint([x, y], true, true, true);
            } else {
                chartT.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/temperature/last", true);
    xhttp.send();
}

function update_humidity_chart() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            var x = data["timestamp"]*1000,
                y = data["value"];
            if (chartH.series[0].data.length > 96) {
                chartH.series[0].addPoint([x, y], true, true, true);
            } else {
                chartH.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/humidity/last", true);
    xhttp.send();
}

function update_moisturepc_chart() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            var x = data["timestamp"]*1000,
                y = data["value"];
            if (chartMP.series[0].data.length > 96) {
                chartMP.series[0].addPoint([x, y], true, true, true);
            } else {
                chartMP.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/moisturepc/last", true);
    xhttp.send();
}

function update_moisturevl_chart() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            var x = data["timestamp"]*1000,
                y = data["value"];
            if (chartMV.series[0].data.length > 96) {
                chartMV.series[0].addPoint([x, y], true, true, true);
            } else {
                chartMV.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/moisturevl/last", true);
    xhttp.send();
}

var chartT = init_chart_data("chart-temperature", "DHT22 Temperature", "Temperature (Celsius)");
var chartH = init_chart_data("chart-humidity", "DHT22 Humidity", "Relative Humidity (%)");
var chartMP = init_chart_data("chart-moisturepc", "Soil Moisture", "Soil Moisture (%)");
var chartMV = init_chart_data("chart-moisturevl", "Soil Moisture", "Soil Moisture (A.U.)");

update_temperature_chart();
update_humidity_chart();
update_moisturepc_chart();
update_moisturevl_chart();

setInterval(update_temperature_chart, 1*60*1000);
setInterval(update_humidity_chart, 1*60*1000);
setInterval(update_moisturepc_chart, 1*60*1000);
setInterval(update_moisturevl_chart, 1*60*1000);