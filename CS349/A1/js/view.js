'use strict';

// Put your view code here (e.g., the graph renderering code)
// Define our views

Number.prototype.pad = function() {
    return (this < 10) ? '0' + this : this;
};


var tableView = function(action, datetime, datapoint) {
    var table = document.getElementById("tableBody");
    var rows = table.getElementsByTagName("tr");

    var count = rows.length;

    if(action == ACTIVITY_DATA_ADDED_EVENT) {
        var updated = false;

        for(var i = 0; i < count && !updated; ++i) {
            var cols = rows[i].getElementsByTagName("td");

            if(cols[0].innerHTML == datapoint.activityType) {
                cols[1].innerHTML = parseInt(cols[1].innerHTML) + datapoint.activityDurationInMinutes;
                updated = true;
            }
        }

        if(!updated) {
            var col0 = document.createElement("td");
            var col1 = document.createElement("td");
            var row = document.createElement("tr");

            col0.innerHTML = datapoint.activityType;
            col1.innerHTML = datapoint.activityDurationInMinutes;

            row.appendChild(col0);
            row.appendChild(col1);
            table.appendChild(row);
        }
    }
};


function updateGraph() {
    var canvas = document.getElementById("graphCanvas");
    var ctx = canvas.getContext('2d');

    // 1: Lazily recreate the basis to erase the graph
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.imageSmoothingEnabled = true;

    var height = 250 * (Math.sqrt(3)/2);

    var X = canvas.width/2;
    var Y = (300-height)/2;

    ctx.beginPath();
    ctx.moveTo(X, Y);
    ctx.lineTo(X+125, Y+height);
    ctx.lineTo(X-125, Y+height);
    ctx.lineTo(X, Y);
    ctx.stroke();
    ctx.closePath();

    ctx.moveTo(X, Y);
    ctx.lineTo(X, Y+(height/2));
    ctx.lineTo(X+125, Y+height);
    ctx.moveTo(X, Y+(height/2));
    ctx.lineTo(X-125, Y+height);
    ctx.stroke();

    ctx.font="15px Georgia";
    ctx.fillText("Happiness",canvas.height/2+4,29);
    ctx.fillText("Stress",X+131,Y+height+14);
    ctx.fillText("Energy",X-179,Y+height+14);

    // 2: Gather data
    var select = document.getElementById("activitySelect");
    var sum = {
        "energy": 0,
        "stress": 0,
        "happiness": 0,
        "duration": 0
    };

    _.each(select.options, function(o) {
        if(o.selected) {
            _.each(sum, function(e, k) {
               sum[k] += parseInt(o.dataset[k]);
            });
        }
    });

    var average = { };
    _.each(sum, function(e, k) {
        average[k] = e / sum.duration;
    });

    // 3: Trace
    var delta = [0, (height/20)];
    ctx.beginPath();
    ctx.moveTo(X + delta[0] * average.happiness, Y+(height/2) - delta[1] * average.happiness); // Happiness

    delta = [125/10, (height/20)];
    ctx.lineTo(X - delta[0] * average.energy, Y+(height/2) + delta[1] * average.energy); // Energy
    ctx.lineTo(X + delta[0] * average.stress, Y+(height/2) + delta[1] * average.stress); // Stress

    ctx.globalAlpha = .9;
    ctx.fill();

    ctx.closePath();
}

var graphView = function(action, datetime, datapoint) {
    var updated = false;
    var select = document.getElementById("activitySelect");
    var count = select.options.length;
    var t = parseInt(datapoint.activityDurationInMinutes);

    for(var i = 0; i < count && !updated; ++i) {
        if(select.options[i].innerHTML == datapoint.activityType) {
            var values = {
                "energy" : parseInt(select.options[i].dataset.energy) + t * parseInt(datapoint.activityDataDict.energyLevel),
                "happiness" : parseInt(select.options[i].dataset.happiness) + t * parseInt(datapoint.activityDataDict.happinessLevel),
                "stress" : parseInt(select.options[i].dataset.stress) + t * parseInt(datapoint.activityDataDict.stressLevel),
                "duration" : parseInt(select.options[i].dataset.duration) + t
            };

            select.options[i].dataset.energy = values.energy;
            select.options[i].dataset.happiness = values.happiness;
            select.options[i].dataset.stress = values.stress;
            select.options[i].dataset.duration = values.duration;

            updated = true;
        }
    }

    if(!updated) {
        var option = document.createElement("option");
        option.value = count;
        option.innerHTML = datapoint.activityType;
        option.selected = true;

        option.dataset.energy = t * parseInt(datapoint.activityDataDict.energyLevel);
        option.dataset.happiness = t * parseInt(datapoint.activityDataDict.happinessLevel);
        option.dataset.stress = t * parseInt(datapoint.activityDataDict.stressLevel);
        option.dataset.duration = t;

        select.appendChild(option);
    }

    updateGraph();
};

var lastEntryView = function(action, datetime) {
    var lastEntry = document.getElementById("lastEntry");

    datetime = new Date(datetime);
    var time = datetime.getHours().pad() + ":" + datetime.getMinutes().pad() + ":" + datetime.getSeconds().pad();

    lastEntry.className = "";

    lastEntry.getElementsByTagName("span")[0].innerHTML = time;
};

var buttonsView = function(action) {
    var buttons = document.getElementById("buttons");
    var analysisBtn = document.getElementById("analysisBtn");

    if(action == ACTIVITY_DATA_ADDED_EVENT) {
        buttons.className = "";
        analysisBtn.click();
    }
};

var selectGraphView = function(action, time, graph) {
    var toggle = function(g) {
        if(g.getAttribute("data-graph") == graph) {
            g.className = "";
        }
        else {
            g.className = "hidden";
        }
    };

    var viewsDiv = document.getElementById("viewsDiv");
    var graphs = viewsDiv.getElementsByTagName("section");

    _.each(graphs, toggle);

    var displayOptions = document.getElementById("displayOptions");
    graphs = displayOptions.getElementsByTagName("section");

    _.each(graphs, toggle);
};