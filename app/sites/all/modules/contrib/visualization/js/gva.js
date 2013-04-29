(function($) {

Drupal.behaviors.visualization_gva = {
  attach: function(context) {
    $.each($(".visualization-chart-gva", context).not(".visualization-processed"), function(idx, value) {
      var chart_id = $(value).attr("id");
      var chart = Drupal.settings.visualization[chart_id];

      $(value).addClass("visualization-processed");

      if (chart !== undefined) {
        function drawChart() {
          var data = google.visualization.arrayToDataTable(chart.dataArray);
          if (data.getNumberOfRows() == 0) {
            var emptyRow = [];

            for (i = 0; i < data.getNumberOfColumns(); i ++) {
              if (i > 0) {
                data.z[i]['type'] = 'number';

                emptyRow.push(0);
              } else {
                emptyRow.push('');
              }
            }

            data.addRow(emptyRow);
          }

          var chartElement = document.getElementById(chart.chart_id);

          switch (chart.type) {
            case 'line':
              Drupal.visualization.charts[chart_id] = new google.visualization.LineChart(chartElement);
              break;

            case 'pie':
              Drupal.visualization.charts[chart_id] = new google.visualization.PieChart(chartElement);
              break;

            case 'column':
              Drupal.visualization.charts[chart_id] = new google.visualization.ColumnChart(chartElement);
              break;

            case 'map':
              chart.options['height'] = 600;
              Drupal.visualization.charts[chart_id] = new google.visualization.GeoMap(chartElement);
              break;
          }

          if (Drupal.visualization.charts[chart_id] !== undefined) {
            Drupal.visualization.charts[chart_id].resize = function(width, height) {
              if (width !== undefined) {
                chart.options['width'] = width;
              }

              if (height !== undefined) {
                chart.options['height'] = height;
              }

              this.draw(data, chart.options);
            }

            Drupal.visualization.charts[chart_id].draw(data, chart.options);
          }
        }

        google.setOnLoadCallback(drawChart);
      }
    });
  },
  detach: function(context) {
  }
}

})(jQuery);
