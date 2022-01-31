package main

import (
	"bytes"
	"encoding/csv"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strings"

	"golang.org/x/text/encoding/unicode"
	"golang.org/x/text/transform"
)

const ItemNamesPath = "../source/ItemName.msg.17.txt"
const SkillNamesPath = "../source/PlayerSkill_Name.msg.17.txt"
const ItemNamesCsvPath = "../items_English.csv"
const SkillNamesCsvPath = "../skills_English.csv"

func readNames(filePath string) []string {

	raw, err := ioutil.ReadFile(filePath)
	if err != nil {
		log.Fatal("Unable to read input file "+filePath, err)
	}

	// Make an tranformer that converts MS-Win default to UTF8:
	win16be := unicode.UTF16(unicode.BigEndian, unicode.IgnoreBOM)
	// Make a transformer that is like win16be, but abides by BOM:
	utf16bom := unicode.BOMOverride(win16be.NewDecoder())

	// Make a Reader that uses utf16bom:
	unicodeReader := transform.NewReader(bytes.NewReader(raw), utf16bom)

	// decode and print:
	decoded, err := ioutil.ReadAll(unicodeReader)
	if err != nil {
		log.Fatal("Unable to read input file "+filePath, err)
	}

	names := strings.Split(string(decoded), "\r\n")
	for i, name := range names {
		names[i] = (strings.Replace(name, "<string>", "", 1))
	}
	names = names[1 : len(names)-1]

	return names
}

func readCsvFile(filePath string) [][]string {
	f, err := os.Open(filePath)
	if err != nil {
		log.Fatalln("Unable to read input file "+filePath, err)
	}
	defer f.Close()

	csvReader := csv.NewReader(f)
	records, err := csvReader.ReadAll()
	if err != nil {
		log.Fatalln("Unable to parse file as CSV for "+filePath, err)
	}

	return records
}

func replaceItemNames(namesCsv [][]string, names, namesInput []string) {
	for row_idx, _ := range namesCsv {
		if row_idx < 2 {
			continue
		}
		if namesCsv[row_idx][2] == names[row_idx-2] {
			namesCsv[row_idx][2] = namesInput[row_idx-2]
		}
	}
}

func replaceSkillNames(namesCsv [][]string, names, namesInput []string) {
	for row_idx, _ := range namesCsv {
		if row_idx < 1 {
			continue
		}
		if namesCsv[row_idx][1] == names[row_idx-1] {
			namesCsv[row_idx][1] = namesInput[row_idx-1]
		}
	}
}

func writeCsvFile(records [][]string, filePath string) {
	f, err := os.Create(filePath)
	if err != nil {
		log.Fatalln("Unable to open file"+filePath, err)
	}
	defer f.Close()

	w := csv.NewWriter(f)
	err = w.WriteAll(records) // calls Flush internally

	if err != nil {
		log.Fatalln("Unalbe to write csv file"+filePath, err)
	}
}

func main() {
	args := os.Args[1:]

	inputLang := args[0]
	itemNamesInputPath := args[1]
	skillNamesInputPath := args[2]

	itemNamesInput := readNames(itemNamesInputPath)
	itemNames := readNames(ItemNamesPath)
	itemNamesCsv := readCsvFile(ItemNamesCsvPath)
	replaceItemNames(itemNamesCsv, itemNames, itemNamesInput)
	writeCsvFile(itemNamesCsv, fmt.Sprintf("../items_%s.csv", inputLang))

	skillNamesInput := readNames(skillNamesInputPath)
	skillNames := readNames(SkillNamesPath)
	skillNamesCsv := readCsvFile(SkillNamesCsvPath)
	replaceSkillNames(skillNamesCsv, skillNames, skillNamesInput)
	writeCsvFile(skillNamesCsv, fmt.Sprintf("../skills_%s.csv", inputLang))

}
