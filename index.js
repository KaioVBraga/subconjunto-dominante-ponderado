const fs = require("fs");
const path = require("path");

const formatNumber = (n, size) => {
  return Number(n.replace("s", "")).toFixed(size).replace(".", ",");
};

const main = async () => {
  const testsData = fs.readFileSync(
    path.join(__dirname, "formated_tests_data.txt"),
    {
      encoding: "utf-8",
    }
  );

  const literatureData = fs.readFileSync(
    path.join(__dirname, "literature.txt"),
    {
      encoding: "utf-8",
    }
  );

  const testsDataObj = testsData.split("\n\n").map((s) =>
    s.split("\n").reduce((sum, v) => {
      const [key, value] = v.split(": ");

      if (!key) {
        return sum;
      }

      sum[key] = value;
      return sum;
    }, {})
  );

  const literatureDataObj = literatureData.split("\n\n").map((s) =>
    s.split("\n").reduce((sum, v) => {
      const [key, value] = v.split(": ");

      if (!key) {
        return sum;
      }

      sum[key.trim()] = value.trim();
      return sum;
    }, {})
  );

  const formatedTestsDataObj = testsDataObj
    .map((data) => {
      console.log("INSTANCE -> ", data["Instance"]);

      const cost = data["Cost"];

      const bestCost = literatureDataObj.find(
        (d) => d["Instance"] === data["Instance"]
      )?.Cost;

      return {
        ...data,
        ["Best Cost"]: bestCost,
        ["Comparison Rate"]: (
          (Number(cost) - Number(bestCost)) /
          Number(bestCost)
        ).toFixed(2),
      };
    })
    .sort((a, b) => a["Instance"].localeCompare(b["Instance"]));

  const formatedTestsData = formatedTestsDataObj
    .map((v) =>
      Object.entries(v)
        .map(([key, value]) => `${key}: ${value}`)
        .join("\n")
    )
    .join("\n\n");

  const latexTable = literatureDataObj
    .map((data) => {
      const greedyData = formatedTestsDataObj.find(
        (d) =>
          d["Instance"] == data["Instance"] && d["Type"] == "Algoritmo Guloso"
      );

      const greedyRandom015Data = formatedTestsDataObj.find(
        (d) =>
          d["Instance"] == data["Instance"] &&
          d["Type"] == "Algoritmo Guloso Randomizado"
      );

      const greedyRandom030Data = formatedTestsDataObj.find(
        (d) =>
          d["Instance"] == data["Instance"] &&
          d["Type"] == "Algoritmo Guloso Randomizado"
      );

      const greedyRandom050Data = formatedTestsDataObj.find(
        (d) =>
          d["Instance"] == data["Instance"] &&
          d["Type"] == "Algoritmo Guloso Randomizado"
      );

      const greedyReactiveData = formatedTestsDataObj.find(
        (d) =>
          d["Instance"] == data["Instance"] &&
          d["Type"] == "Algoritmo Guloso Randomizado Reativo"
      );

      const name = data["Instance"].replace(/\_/g, "\\_");
      const bestCost = formatNumber(data["Cost"], 2);
      //   const time = formatNumber(data["Time"], 3);

      //   return `Problem.dat\_50\_50\_3   & 0,0   & {\\bf 0,00}    & 0,282   & {\\bf 0,00}   & 0,141   & {\\bf 0,00}     & \\textbf{0,114}  \\\\ \\hline`;
      //   return `${name} & ${bestCost} & ${formatNumber(
      //     greedyData["Comparison Rate"],
      //     2
      //   )} & ${formatNumber(
      //     greedyRandom015Data["Comparison Rate"],
      //     2
      //   )} & ${formatNumber(
      //     greedyRandom030Data["Comparison Rate"],
      //     2
      //   )}  & ${formatNumber(
      //     greedyRandom050Data["Comparison Rate"],
      //     2
      //   )} & ${formatNumber(
      //     greedyReactiveData["Comparison Rate"],
      //     2
      //   )}  \\\\ \\hline`;

      return `${name} & ${formatNumber(greedyData["Time"], 5)} & ${formatNumber(
        greedyRandom015Data["Time"],
        5
      )} & ${formatNumber(greedyRandom030Data["Time"], 5)}  & ${formatNumber(
        greedyRandom050Data["Time"],
        5
      )} & ${formatNumber(greedyReactiveData["Time"], 5)}  \\\\ \\hline`;
    })
    .join("\n");

  fs.writeFileSync(path.join(__dirname, "latex_table.txt"), latexTable);

  //   fs.writeFileSync(
  //     path.join(__dirname, "compared_data.txt"),
  //     formatedTestsData
  //   );
};

main();
