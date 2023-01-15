const fs = require("fs");
const path = require("path");

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

  const formatedTestsDataObj = testsDataObj.map((data) => {
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
  });

  const formatedTestsData = formatedTestsDataObj
    .map((v) =>
      Object.entries(v)
        .map(([key, value]) => `${key}: ${value}`)
        .join("\n")
    )
    .join("\n\n");

  fs.writeFileSync(
    path.join(__dirname, "compared_data.txt"),
    formatedTestsData
  );
};

main();
