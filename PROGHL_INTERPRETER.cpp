#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <cstdlib>
using namespace std;

void remove_spaces(const std::string& input_file_name, const std::string& output_file_name){
    std::ifstream input(input_file_name);
    std::ofstream output(output_file_name);

    if (!input.is_open()){
        std:cerr << "ERROR opening file."<<std::endl;
        return;
    }

    char character;
    while (input.get(character)){
        if(!std::isspace(character)){
            output<<character;
        }
    }
    input.close();
    output.close();

    std::cout<<"Whitespace removed."<<std::endl;
}

enum Token_type {
    IDENTIFIER, 
    KEYWORD,    
    INT_VALUE,
    DOUBLE_VALUE,    
    OPERATOR,   
    DELIMITER,  
    UNKNOWN
};

struct Token {
    string value;
    Token_type type;
    size_t position;
};

vector<Token> tokens;
int line = 0;
int total_checks = 0;
int checker_1 = 0;
int checker_2 = 0;
int checker_3 = 0;
int checker_4 = 0;
int checker_5 = 0;
int checker_6 = 0;
int initial_index = 0;
int size;
std::string file;
std::string output_value;

double left_value = 0.0;
double right_value = 0.0;

bool conditionMet = false;

std::string getTokenTypeName(Token_type type) {
    switch (type) {
        case Token_type::KEYWORD:
            return "KEYWORD";
        case Token_type::IDENTIFIER:
            return "IDENTIFIER";
        case Token_type::INT_VALUE:
            return "INT_VALUE";
        case Token_type::DOUBLE_VALUE:
            return "DOUBLE_VALUE";
        case Token_type::DELIMITER:
            return "DELIMITER";
        case Token_type::OPERATOR:
            return "OPERATOR";
        case Token_type::UNKNOWN:
            return "UNKNOWN";

        default:
            return "INVALID"; 
    }
}

void tokenizeAndProcess(std::string input_file_name, std::string outputFileName) {
    std::regex keyword_regex("\\b(if|integer|double|output)\\b");
    std::regex identifier_regex("[a-zA-Z_][a-zA-Z0-9_]*");
    std::regex int_regex("\\b\\d(?!\\d|\\.)\\b");
    std::regex double_regex("\\d\\.\\d{2}");
    std::regex operator_regex("==|!=|<=|>=|<<|[=+<>-]");
    std::regex delimiter_regex("[()\\:;]");

    std::ifstream inputFile(input_file_name);
    std::ofstream outputFile(outputFileName);
    std::string line;

    while (std::getline(inputFile, line)) {

        std::sregex_iterator it(line.begin(), line.end(), keyword_regex);
        std::sregex_iterator end;

        while (it != end) {
            Token token{it->str(), KEYWORD, static_cast<size_t>(it->position())};
            tokens.push_back(token);
            ++it;
        }

        it = std::sregex_iterator(line.begin(), line.end(), int_regex);
        while (it != end) {
            Token token{it->str(), INT_VALUE, static_cast<size_t>(it->position())};
            tokens.push_back(token);
            ++it;
        }

        it = std::sregex_iterator(line.begin(), line.end(), double_regex);
        while (it != end) {
            Token token{it->str(), DOUBLE_VALUE, static_cast<size_t>(it->position())};
            tokens.push_back(token);
            ++it;
        }

        it = std::sregex_iterator(line.begin(), line.end(), operator_regex);
        while (it != end) {
            Token token{it->str(), OPERATOR, static_cast<size_t>(it->position())};
            tokens.push_back(token);
            ++it;
        }

        it = std::sregex_iterator(line.begin(), line.end(), identifier_regex);
        while (it != end) {
        std::string token_str = it->str();

        if (token_str != "integer" && token_str != "if" && token_str != "double" && token_str != "output") {
            Token token{token_str, IDENTIFIER, static_cast<size_t>(it->position())};
            tokens.push_back(token);
        }

        ++it;
        }

        it = std::sregex_iterator(line.begin(), line.end(), delimiter_regex);
        while (it != end) {
            Token token{it->str(), DELIMITER, static_cast<size_t>(it->position())};
            tokens.push_back(token);
            ++it;
        }

        sort(tokens.begin(), tokens.end(), [](const Token& a, const Token& b) {
            return a.position < b.position;
        });

        for (const Token& token : tokens) {
            switch (token.type) {
                case KEYWORD:
                    outputFile << "Keyword: " << token.value << std::endl;
                    break;
                case INT_VALUE:
                    outputFile << "Int: " << token.value << std::endl;
                    break;
                case DOUBLE_VALUE:
                    outputFile << "Double: " << token.value << std::endl;
                    break;
                case OPERATOR:
                    outputFile << "Operator: " << token.value << std::endl;
                    break;
                case IDENTIFIER:
                    outputFile << "Identifier: " << token.value << std::endl;
                    break;
                case DELIMITER:
                    outputFile << "Delimiter: " << token.value << std::endl;
                    break;
                default:
                    break;
            }
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Lexer complete. \n" << std::endl;
    // std::cout<<"--------------LEXEMES-------------"<<std::endl;
    // for (const Token& token : tokens){
    // std::cout << "\nValue: " << token.value << "\nType: " << getTokenTypeName(token.type) << std::endl;
    // }
}

Token next_token(std::vector<Token>& tokens, size_t& current_index) {
    if (current_index < tokens.size()) {
        return tokens[current_index++];
    } else {

        return {"", Token_type::UNKNOWN};
    }
}

enum class Data_type { INTEGER, DOUBLE };
struct Variable {
    Data_type type;
    std::string name;
    double value;  
};

std::vector<Variable> variables;  

bool parse_declaration(std::vector<Token>& tokens, size_t& current_index) {

    Token varName = next_token(tokens, current_index);

    if (varName.type == Token_type::IDENTIFIER) {

        Token colon = next_token(tokens, current_index);

        checker_1 ++;

        if (colon.type == Token_type::DELIMITER && colon.value == ":") {

            Token dataType = next_token(tokens, current_index);

            checker_1 ++;

            if (dataType.type == Token_type::KEYWORD && (dataType.value == "integer" || dataType.value == "double")) {

                Token semicolon = next_token(tokens, current_index);

                checker_1 ++;

                if (semicolon.type == Token_type::DELIMITER && semicolon.value == ";") {

                	Variable newVariable;
                    newVariable.name = varName.value;
                    newVariable.type = (dataType.value == "integer") ? Data_type::INTEGER : Data_type::DOUBLE;
                    variables.push_back(newVariable);
                	checker_1 ++;                	
                }
            }
        }
    }

    if (checker_1 == 4){
    	initial_index = current_index;    	
    	return true;
	}else{
		current_index =  initial_index;

		checker_1 = 0;
		return false;
	}

}

bool parse_expression(std::vector<Token>& tokens, size_t& current_index) {

    Token varName = next_token(tokens, current_index);

    if (varName.type == Token_type::IDENTIFIER) {

        Token colonDel = next_token(tokens, current_index);

        checker_2 ++;

        if (colonDel.type == Token_type::DELIMITER && colonDel.value == ":") {

            Token equalOp = next_token(tokens, current_index);

            checker_2 ++;

            if (equalOp.type == Token_type::OPERATOR && equalOp.value == "=") {

            	Token expression = next_token(tokens, current_index);

            	checker_2 ++;

	            if (expression.type == Token_type::INT_VALUE || expression.type == Token_type::DOUBLE_VALUE) {

	                Token semicolon = next_token(tokens, current_index);

	                checker_2 ++;

	                if (semicolon.type == Token_type::DELIMITER && semicolon.value == ";") {

	                    for (Variable& variable : variables) {
	                        if (variable.name == varName.value) {
	                            if (expression.type == Token_type::INT_VALUE) {
	                                if (variable.type == Data_type::INTEGER) {
	                                    variable.value = std::stoi(expression.value);
	                                } else if (variable.type == Data_type::DOUBLE) {

	                                    std::cout<<"INCORRECT DATATYPE"<<std::endl;
	                                    break;
	                                }
	                            } else if (expression.type == Token_type::DOUBLE_VALUE) {
	                                if (variable.type == Data_type::INTEGER) {
	                                    std::cout<<"INCORRECT DATATYPE"<<std::endl;
	                                    break;
	                                } else if (variable.type == Data_type::DOUBLE) {
	                                    variable.value = std::stod(expression.value);
	                                }
	                            }

	                        }
	                    }
	                    checker_2 ++;
						} 
					}
				}
			}	
        }

    if (checker_2 == 5){
    	initial_index = current_index;
    	return true;
	}else{
		current_index =  initial_index;

		checker_2 = 0;
		return false;
	}
}

bool parse_standard_output(std::vector<Token>& tokens, size_t& current_index) {

    Token outputKeyword = next_token(tokens, current_index);

    if (outputKeyword.type == Token_type::KEYWORD && outputKeyword.value == "output") {

        Token outputOperator = next_token(tokens, current_index);

        checker_3 ++;

        if (outputOperator.type == Token_type::OPERATOR && outputOperator.value == "<<") {

            Token expression = next_token(tokens, current_index);

            checker_3 ++;

            if (expression.type == Token_type::DOUBLE_VALUE || expression.type == Token_type::INT_VALUE || expression.type == Token_type::IDENTIFIER) {

                Token semicolon = next_token(tokens, current_index);

                checker_3 ++;

                if (semicolon.type == Token_type::DELIMITER && semicolon.value == ";") {

                    checker_3 ++;          

					if (expression.type == Token_type::DOUBLE_VALUE || expression.type == Token_type::INT_VALUE) {

                    std::cout << expression.value;
                		} else if (expression.type == Token_type::IDENTIFIER) {

                    	for (const Variable& variable : variables) {
                        if (variable.name == expression.value) {
                            std::cout << variable.value << std::endl;

                            break;
                       		 }
                   		 }
               			 }         
                	}
            	}
			}
        }

    if (checker_3 == 4){
    	initial_index = current_index;
    	return true;
	}else{
		current_index =  initial_index;

		checker_3 = 0;
		return false;

	}
}

bool parse_conditional(std::vector<Token>& tokens, size_t& current_index) {

    Token ifKeyword = next_token(tokens, current_index);

    if (ifKeyword.type == Token_type::KEYWORD && ifKeyword.value == "if") {

        Token openParenthesis = next_token(tokens, current_index);

        checker_4 ++;

        if (openParenthesis.type == Token_type::DELIMITER && openParenthesis.value == "(") {

            Token lhsToken = next_token(tokens, current_index);

            checker_4 ++;

            if (lhsToken.type == Token_type::IDENTIFIER || lhsToken.type == Token_type::INT_VALUE || lhsToken.type == Token_type::DOUBLE_VALUE) {
                Token comparisonOperator = next_token(tokens, current_index);

                checker_4 ++;
					if(lhsToken.type == Token_type:: IDENTIFIER){
					    for (const Variable& variable : variables) {
		                    if (variable.name == lhsToken.value) {
					        	left_value = variable.value;
					        	break;
					        }
					    }
						}else{
							left_value = std::stod(lhsToken.value);
						}

                if (comparisonOperator.type == Token_type::OPERATOR &&
                    (comparisonOperator.value == ">" || comparisonOperator.value == "<" ||
                     comparisonOperator.value == "==" || comparisonOperator.value == "!=")) {

                    Token rhsToken = next_token(tokens, current_index);

                    checker_4 ++;

                    	if (rhsToken.type == Token_type::IDENTIFIER || rhsToken.type == Token_type::INT_VALUE || rhsToken.type == Token_type::DOUBLE_VALUE) {
                    	Token closeParenthesis = next_token(tokens, current_index);

                		checker_4 ++;
                		if(rhsToken.type == Token_type:: IDENTIFIER){
					    for (const Variable& variable : variables) {
		                    if (variable.name == rhsToken.value) {
					        	right_value = variable.value;
					        	break;
					        }
					    }
						}else{
							right_value = std::stod(rhsToken.value);
						}

                			if (closeParenthesis.type == Token_type::DELIMITER && closeParenthesis.value == ")") {

            				checker_4 ++;

            				if (comparisonOperator.value == "<") {
            					conditionMet = left_value < right_value;

			            		} else if (comparisonOperator.value == ">"){
			            			conditionMet = left_value > right_value;
								} else if (comparisonOperator.value == "=="){
									conditionMet = left_value == right_value;
								} else if (comparisonOperator.value == "!="){
									conditionMet = left_value != right_value;
								}
							if (conditionMet == false){
							while(true){
								Token stop = next_token(tokens, current_index);
								size -= 1;
								if (stop.value == ";"){
									break;
							}
							}
							}
                        }
                    }
                }
            }
        }
    }

    if (checker_4 == 6){
    	initial_index = current_index;
    	return true;
	}else{
		current_index =  initial_index;

		checker_4 = 0;
		return false;
	}
	}

bool parse_arithmetic(std::vector<Token>& tokens, size_t& current_index) {

    Token varName = next_token(tokens, current_index);

    if (varName.type == Token_type::IDENTIFIER) {

        Token equals = next_token(tokens, current_index);

        checker_5 ++;

        if (equals.type == Token_type::OPERATOR && equals.value == "=") {
        double result = 0.0;

        Token num = next_token(tokens, current_index);

        checker_5 ++;

		    if (num.type == Token_type::INT_VALUE || num.type == Token_type::DOUBLE_VALUE || num.type == Token_type::IDENTIFIER) {
		    	if (num.type == Token_type::INT_VALUE) {
                	result = std::stod(num.value);
            		} else if (num.type == Token_type::DOUBLE_VALUE) {
                		result = std::stod(num.value);
            		} else if (num.type == Token_type::IDENTIFIER) {

               	 	for (const Variable& variable : variables) {
                    if (variable.name == num.value) {
                        result = variable.value;
                        break;
                    }
                }
            	}

		        Token mathOp = next_token(tokens, current_index);

		        checker_5 ++;

		        if (mathOp.type == Token_type::OPERATOR && (mathOp.value == "+" || mathOp.value == "-")) {

		            Token num2 = next_token(tokens, current_index);

		            checker_5 ++;

				    if (num2.type == Token_type::INT_VALUE || num2.type == Token_type::DOUBLE_VALUE || num.type == Token_type::IDENTIFIER) {

				        Token finalColon = next_token(tokens, current_index);

				        checker_5 ++;
				         if (num2.type == Token_type::INT_VALUE) {
                    double num2Value = std::stod(num2.value);

                    if (mathOp.value == "+") {
                        result += num2Value;
                    } else {
                        result -= num2Value;
                    }
                } else if (num2.type == Token_type::DOUBLE_VALUE) {
                    double num2Value = std::stod(num2.value);

                    if (mathOp.value == "+") {
                        result += num2Value;
                    } else {
                        result -= num2Value;
                    }
                } else if (num2.type == Token_type::IDENTIFIER) {

                    for (const Variable& variable : variables) {
                        if (variable.name == num2.value) {
                            if (mathOp.value == "+") {
                                result += variable.value;
                            } else {
                                result -= variable.value;
                            }
                            break;
                        }
                    }
                }

            for (Variable& variable : variables) {
                if (variable.name == varName.value) {
                    variable.value = result;
                    break;
                }
            }

		                if (finalColon.type == Token_type::DELIMITER && finalColon.value == ";") {

		                    checker_5 ++;   
								}
		        	        }
		    	        }                   

		        	}
		    	}
		    }

   if (checker_5 == 6){
    	initial_index = current_index;
    	return true;
	}else{
		current_index =  initial_index;

		checker_5 = 0;
		return false;
	}
}

bool parse_print(std::vector<Token>& tokens, size_t& current_index) {

    Token outputKeyword = next_token(tokens, current_index);

    if (outputKeyword.type == Token_type::KEYWORD && outputKeyword.value == "output") {

        Token outputOperator = next_token(tokens, current_index);

        checker_6 ++;

        if (outputOperator.type == Token_type::OPERATOR && outputOperator.value == "<<") {

            Token operand1 = next_token(tokens, current_index);

            checker_6 ++;

            if (operand1.type == Token_type::IDENTIFIER || operand1.type == Token_type::INT_VALUE || operand1.type == Token_type::DOUBLE_VALUE) {
            	double result = 0;

            	Token operator1 = next_token(tokens, current_index);

            	checker_6 ++;

            		if (operator1.type == Token_type::OPERATOR && (operator1.value == "+" || operator1.value == "-")) {

            		Token operand2 = next_token(tokens, current_index);

            		checker_6 ++;

	            		if (operand2.type == Token_type::IDENTIFIER || operand2.type == Token_type::INT_VALUE || operand2.type == Token_type::DOUBLE_VALUE) {
	            			double value1 = 0;
                    		double value2 = 0;

	            		Token semicolon = next_token(tokens, current_index);

	            		checker_6 ++;

	            			if (semicolon.type == Token_type::DELIMITER && semicolon.value == ";") {

	            			checker_6 ++;
	            			}
	            	if (operand1.type == Token_type::IDENTIFIER) {

                        for (const Variable& variable : variables) {
                        if (variable.name == operand1.value) {
                            value1 = variable.value;
                            break;
                        }
                    }
                    } else if (operand1.type == Token_type::INT_VALUE) {
                        value1 = std::stoi(operand1.value);
                    } else if (operand1.type == Token_type::DOUBLE_VALUE) {
                        value1 = std::stod(operand1.value);
                    }

                    if (operand2.type == Token_type::IDENTIFIER) {

                        for (const Variable& variable : variables) {
                        if (variable.name == operand2.value) {
                            value2 = variable.value;
                            break;
                        }
                    }
                    } else if (operand2.type == Token_type::INT_VALUE) {
                        value2 = std::stoi(operand2.value);
                    } else if (operand2.type == Token_type::DOUBLE_VALUE) {
                        value2 = std::stod(operand2.value);
                    }

                    if (operator1.value == "+") {
                        result = value1 + value2;
                    } else if (operator1.value == "-") {
                        result = value1 - value2;
                    }
                    std::cout << result;

            			}
            		}
            	}
			}
        }

    if (checker_6 == 6){
    	initial_index = current_index;
    	return true;
	}else{
		current_index =  initial_index;

		checker_6 = 0;
		return false;
	}
	}

bool parse_code(std::vector<Token>& tokens) {
    size_t current_index = 0;  
    int newest_index = 0;

    while (current_index < tokens.size()) {

        if(parse_declaration(tokens, current_index)){

        	total_checks += checker_1;
        	checker_1 = 0;
		} else if(parse_expression(tokens, current_index)){	

			total_checks += checker_2;
        	checker_2 = 0;
		} else if (parse_standard_output(tokens, current_index)){

			total_checks += checker_3;
        	checker_3 = 0;
		} else if (parse_conditional(tokens, current_index)){

			total_checks += checker_4;
        	checker_4 = 0;
		} else if (parse_arithmetic(tokens, current_index)){

			total_checks += checker_5;
        	checker_5 = 0;
        } else if (parse_print(tokens, current_index)){

			total_checks += checker_6;
        	checker_6 = 0;
        	checker_5 = 0;
		}else{

			break;
		}
	}

    checker_1 = 0;
    checker_2 = 0;
    checker_3 = 0;
    checker_4 = 0;
    checker_5 = 0;
    checker_6 = 0;
    line = 0;

    if(total_checks == size){
    	return true;
    } else {
    	return false;
	}
	}

int main() {
	std::cout<<"Enter filename of code file, make sure it is in the same directory as the interpeter:";
	std::cin>>file;
	remove_spaces(file, "NOSPACES.txt");

	tokenizeAndProcess("NOSPACES.txt", "RES_SYM.txt");
	size = tokens.size();
	// std::cout<<"\n-------------OUTPUT-------------\n"<<std::endl;
	bool parse_result = parse_code(tokens);

    if (parse_result) {
        std::cout <<"\n\n--------------------------------\n\nNO ERROR(S)." << std::endl;
        total_checks = 0;
    } else {
    	system("cls");
        std::cout <<"\n--------------------------------\n\nERROR" <<std::endl;
        total_checks = 0;
    }      
    std::cout << "press any key to exit...";                                         
    return 0;
}