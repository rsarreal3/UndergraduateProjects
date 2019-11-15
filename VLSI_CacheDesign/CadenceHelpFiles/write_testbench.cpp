///////////////////////////////////////////////////////////////////////////////////////////////
// File Name:     write_testbench.cpp
// Author:        Dang-Quang Tran (dtran6@umbc.edu)
// Description:   A program that writes a testbench for an entity specified in the .vhdl file.
// Last Revision: 11/16/2017
///////////////////////////////////////////////////////////////////////////////////////////////

// Refer to the setup instructions in the execute.bash script for setup instructions.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace std;

const string CLOCK_NAME = "clk";

class Port {

  public:
  
  Port() {
    name = "";
    direction = "in";
  }
  
  Port(string name, string direction) {
    this->name = name;
    this->direction = direction;
  }
  
  virtual Port* clone() {
    return new Port(*this);
  }
  
  string getName() const {
    return name;
  }
  
  string getDirection() const {
    return direction;
  }
  
  virtual int getNumPins() const {
    return 1;
  }
  
  virtual string toSignal() const {
    return "signal " + name + ": std_logic;";
  }
  
  protected:
  
  string name;
  string direction;
  
};

class Bus : public Port {
  
  public:
  
  Bus() : Port("", "in") {}
  
  Bus(string name, string direction, int numPins)
  : Port(name, direction) {
    this->numPins = numPins;
  }
  
  virtual Port* clone() {
    return new Bus(*this);
  } 
  
  string toSignal() const {
    stringstream msbss, lsbss;
    msbss << numPins - 1;
    lsbss << 0;
    return "signal " + name + ": std_logic_vector(" + msbss.str() + " downto " + lsbss.str() + ");";
  }
  
  int getNumPins() const {
    return numPins;
  }
  
  private:
  
  int numPins;
  
};

string generateHeaderComment(string testbenchName, string author) {
  string headerComment = "";
  
  headerComment += "--\n-- Entity: " + testbenchName + "\n-- Architecture: vhdl\n";
  headerComment += "-- Author: " + author + "\n--\n";
  
  return headerComment;
}

string generateIncludes() {
  return "library IEEE;\nuse IEEE.std_logic_1164.all;\nuse IEEE.std_logic_textio.all;\nuse IEEE.std_logic_arith.all;\nuse STD.textio.all;\n";
}

string generateRunningSimulationHeader(int numInputBits) {
  string header = "";
  
  stringstream simulationTime;
  simulationTime << 2 * pow(2, numInputBits);
  
  header += "    write(out_line, string'(\"Running all possible combinations. Will take some time to run!\"));\n";
  header += "    writeline(output, out_line);\n";
  header += "    write(out_line, string'(\"Type 'run " + simulationTime.str() + " ns' to continue or 'exit' to quit\"));\n";
  header += "    writeline(output, out_line);\n";
  header += "    write(out_line, string'(\"Redirect output using > if required and multiple ^C's to quit\"));\n";
  header += "    writeline(output, out_line);\n";
  
  return header;
}

int main(int argc, char** argv) {
  
  if (argc < 2) {
    cout << "Specify .vhdl file of the entity for which to create a testbench." << endl;
    return 1;
  }
  
  string filename = argv[1];
  ifstream entityFile(filename.c_str());
  
  string componentPorts = "";
  vector<Port *> ports;
  
  string line, token;
  
  string entityName;
  
  int numInputBits = 0;
    
  while (getline(entityFile, line)) {
    stringstream ssline(line);
    
    ssline >> token;
    
    if (token == "entity") {
      ssline >> entityName;
      
      while (getline(entityFile, line)) {
        stringstream ssline(line);
        ssline >> token;
        if (token.substr(0, 4) == "port") {
          break;
        }
      }
      
      string portName = "";
      string direction;
      string stdLogicType = "";
      
      while (getline(entityFile, line)) {
        stringstream ssline(line);
        portName = "";
        stdLogicType = "";

        ssline >> token;
        
        if (token.substr(0, 3) != "end") {
          componentPorts += "  " + line + "\n";
          int i;
          
          for (i = 0; line[i] != ':'; i++) {
            if (!iswspace(line[i])) {
              portName += line[i];
            }
          }
          
          if (portName == "output") {
            portName = "out_put";
          }
          
          stringstream sslineAfterName(line.substr(i + 1, line.length()));
          
          sslineAfterName >> direction;
          sslineAfterName >> stdLogicType;
          
          if (stdLogicType.length() < 16 && stdLogicType.substr(0, 9) == "std_logic") {
            ports.push_back(new Port(portName, direction));
            if (direction == "in") {
              numInputBits++;
            }
          } else if (stdLogicType.length() >= 16 && stdLogicType.substr(0, 16) == "std_logic_vector") {
            int msb, lsb;
            
            for (; line[i] != '('; i++) {}
            
            stringstream range(line.substr(i + 1, line.length()));
                        
            range >> msb >> token >> lsb;
            
            ports.push_back(new Bus(portName, direction, msb - lsb + 1));
            if (direction == "in" || direction == "inout") {
              numInputBits += msb - lsb + 1;
            }
          }
        } else {
          break;
        }
      }
      break;
    }
  }
  
  string testbenchName = entityName + "_test_stdout";
  string testbenchFilename = testbenchName + ".vhdl";
  
  ofstream entityTestbench(testbenchFilename.c_str());
  
  entityTestbench << generateHeaderComment(testbenchName, "dtran6");
  
  entityTestbench << generateIncludes();
  
  entityTestbench << endl;
  
  entityTestbench << "entity " << testbenchName << " is" << endl;
  entityTestbench << endl;
  entityTestbench << "end " << testbenchName << ";" << endl;
  
  entityTestbench << endl;
  
  entityTestbench << "architecture test of " << testbenchName << " is" << endl;
  entityTestbench << endl;
  entityTestbench << "  component " << entityName << endl;
  entityTestbench << "    port (" << endl;
  entityTestbench << componentPorts;
  entityTestbench << "  end component;" << endl;
  
  entityTestbench << endl;
  
  entityTestbench << "  for " << entityName << "_1: " << entityName << " use entity work."
                  << entityName << "(structural);" << endl;
                  
  for (unsigned int i = 0; i < ports.size(); i++) {
    entityTestbench << "  " << ports.at(i)->toSignal() << endl;
  }
  entityTestbench << "  signal create_input: std_logic_vector(" << numInputBits - 1 
                  << " downto 0) := b\"";
  for (int i = 0; i < numInputBits; i++) {
    entityTestbench << "0";
  }
  entityTestbench << "\";" << endl;
  
  entityTestbench << endl;
  
  entityTestbench << "  procedure print_output is" << endl;
  entityTestbench << "    variable out_line: line;" << endl;
  entityTestbench << endl;
  entityTestbench << "    begin" << endl;
  for (unsigned int i = 0; i < ports.size(); i++) {
    entityTestbench << "    write(out_line, string'(\" " << ports.at(i)->getName() << ": \"));" << endl;
    entityTestbench << "    write(out_line, " << ports.at(i)->getName() << ");" << endl;
    entityTestbench << "    writeline(output, out_line);" << endl;
  }
  entityTestbench << "    writeline(output, out_line);" << endl;
  entityTestbench << "  end print_output;" << endl;
  
  entityTestbench << endl;
  
  entityTestbench << "begin" << endl;
  entityTestbench << endl;
  entityTestbench << "  " << entityName << "_1: " << entityName << " port map(";
  for (unsigned int i = 0; i < ports.size(); i++) {
    entityTestbench << ports.at(i)->getName();
    if (i != ports.size() - 1) {
      entityTestbench << ", ";
    }
  }
  entityTestbench << ");" << endl;
  entityTestbench << endl;
  entityTestbench << "  create_input <= unsigned(create_input) + unsigned'(b\"";
  for (int i = 0; i < numInputBits; i++) {
    if (i != numInputBits - 1) {
      entityTestbench << "0";
    } else {
      entityTestbench << "1";
    }
  }
  entityTestbench << "\") after 2 ns;" << endl;
  entityTestbench << endl;
  
  entityTestbench << "  io_process: process" << endl;
  entityTestbench << endl;
  entityTestbench << "    variable out_line : line;" << endl;
  
  entityTestbench << endl;
  
  entityTestbench << "  begin" << endl;
  entityTestbench << generateRunningSimulationHeader(numInputBits);
  
  entityTestbench << "    for i in 0 to " << pow(2, numInputBits) - 1 << " loop" << endl;
  entityTestbench << "      wait for 1 ns;" << endl;
  entityTestbench << endl;
  
  for (unsigned int i = 0; i < ports.size() - 1; i++) {
    for (unsigned int j = 0; j < ports.size() - i - 1; j++) {
      if (ports.at(j)->getNumPins() < ports.at(j + 1)->getNumPins()) {
        Port* newPort = ports.at(j + 1)->clone();
        delete ports.at(j + 1);
        ports.erase(ports.begin() + j + 1);
        ports.insert(ports.begin() + j, newPort);
      }
    }
  }
  bool isSequential = false;
  for (unsigned int i = 0; i < ports.size(); i++) {
    if (ports.at(i)->getName() == CLOCK_NAME && (ports.at(i)->getDirection() == "in" || ports.at(i)->getDirection() == "inout")) {
      ports.push_back(ports.at(i)->clone());
      delete ports.at(i);
      ports.erase(ports.begin() + i);
      isSequential = true;
      break;
    }
  }
  
  for (unsigned int i = 0; i < ports.size(); i++) {
    if (ports.at(i)->getDirection() == "in" || ports.at(i)->getDirection() == "inout") {
      if (ports.at(i)->getName() == CLOCK_NAME) {
        entityTestbench << "      wait for 1 ns;" << endl;
      }
      entityTestbench << "      " << ports.at(i)->getName() << " <= create_input(";
      if (ports.at(i)->getNumPins() == 1) {
        entityTestbench << numInputBits - 1 << ");" << endl;
      } else {
        entityTestbench << numInputBits - 1 << " downto " << numInputBits - ports.at(i)->getNumPins() << ");" << endl;
      }
      numInputBits -= ports.at(i)->getNumPins();
    }
  }
  if (!isSequential) {
    entityTestbench << "      wait for 1 ns;" << endl;
  }
  entityTestbench << endl;
  entityTestbench << "      print_output;" << endl;
  entityTestbench << "    end loop;" << endl;
  
  entityTestbench << endl;
  entityTestbench << "  end process io_process;" << endl;
  
  entityTestbench << endl;
  entityTestbench << "end test;" << endl;
  
  entityTestbench.close();
  
  cout << "Created testbench for entity " << entityName << ": " << testbenchFilename << "." << endl;
  
  for (unsigned int i = 0; i < ports.size(); i++) {
    delete ports.at(i);
  }
  
  return 0;
}