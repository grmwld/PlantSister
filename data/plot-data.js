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
                dataLabels: { enabled: false }
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

function update_chart(method, uri, hichart) {
    return function() {
        var xhttp = new XMLHttpRequest();
        if (hichart.series[0].data.length == 0) {
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    data.forEach(datapoint => {
                        var x = datapoint["timestamp"] * 1000,
                            y = round(datapoint["value"], 1);
                        if (hichart.series[0].data.length > 96) {
                            hichart.series[0].addPoint([x, y], true, true, true);
                        } else {
                            hichart.series[0].addPoint([x, y], true, false, true);
                        }
                    });
                }
            }
            xhttp.open(method, uri + "/history", true);
        } else {
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    var data = JSON.parse(this.responseText);
                    var x = data["timestamp"] * 1000,
                        y = round(data["value"], 1);
                    if (hichart.series[0].data.length > 96) {
                        hichart.series[0].addPoint([x, y], true, true, true);
                    } else {
                        hichart.series[0].addPoint([x, y], true, false, true);
                    }
                }
            }
            xhttp.open(method, uri + "/last", true);
        }
        xhttp.send();
    }
}

function round(value, decimals) {
    return Number(Math.round(value+'e'+decimals)+'e-'+decimals);
}

var chartT = init_chart_data("chart-temperature", "DHT22 Temperature", "Temperature (Celsius)");
var chartH = init_chart_data("chart-humidity", "DHT22 Humidity", "Relative Humidity (%)");
var chartMP = init_chart_data("chart-moisturepc", "Soil Moisture", "Soil Moisture (%)");
var chartMV = init_chart_data("chart-moisturevl", "Soil Moisture", "Soil Moisture (A.U.)");

update_temperature_chart = update_chart("GET", "./temperature", chartT);
update_humidity_chart = update_chart("GET", "./humidity", chartH);
update_moisturepc_chart = update_chart("GET", "./moisturepc", chartMP);
update_moisturevl_chart = update_chart("GET", "./moisturevl", chartMV);

update_temperature_chart();
update_humidity_chart();
update_moisturepc_chart();
update_moisturevl_chart();

setInterval(update_temperature_chart, 5 * 60 * 1000);
setInterval(update_humidity_chart, 5 * 60 * 1000);
setInterval(update_moisturepc_chart, 15 * 60 * 1000);
setInterval(update_moisturevl_chart, 15 * 60 * 1000);