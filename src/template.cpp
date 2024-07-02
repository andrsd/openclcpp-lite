// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "openclcpp-lite/template.h"
#include <iostream>

namespace openclcpp_lite {

// params

Template::Params::Params() {}

// lexer

Template::Lexer::Lexer(const std::string & source) : sstr(source), mode(Text), have_token(false) {}

Template::Token
Template::Lexer::peek()
{
    if (!this->have_token) {
        this->curr = read_token();
        this->have_token = true;
    }
    return this->curr;
}

Template::Token
Template::Lexer::read()
{
    peek();
    this->have_token = false;
    return this->curr;
}

void
Template::Lexer::set_mode(Mode mode)
{
    this->mode = mode;
}

Template::Token
Template::Lexer::read_token()
{
    while (true) {
        if (this->sstr.peek() == EOF) {
            Token t = { Token::End, "" };
            return t;
        }

        std::string str_tok;
        char ch = read_char();
        if (ch == '{') {
            str_tok += ch;
            auto next = peek_char();
            if (next == '{') {
                str_tok += read_char();
                Token token = { Token::OpenVar, str_tok };
                return token;
            }
            else {
                Token token = { Token::Character, str_tok };
                return token;
            }
        }
        else if (ch == '}') {
            str_tok += ch;
            auto next = peek_char();
            if (next == '}') {
                str_tok += read_char();
                Token token = { Token::CloseVar, str_tok };
                return token;
            }
            else {
                Token token = { Token::Character, str_tok };
                return token;
            }
        }
        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            if (this->mode == Var)
                continue;
            else {
                str_tok += ch;
                Token token = { Token::Character, str_tok };
                return token;
            }
        }
        else {
            if (this->mode == Var) {
                str_tok += ch;
                ch = peek_char();
                while (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' && ch != EOF) {
                    str_tok += read_char();
                    ch = peek_char();
                }
                Token token = { Token::Variable, str_tok };
                return token;
            }
            else {
                str_tok += ch;
                Token token = { Token::Character, str_tok };
                return token;
            }
        }
    }
}

char
Template::Lexer::read_char()
{
    char ch;
    this->sstr.read(&ch, sizeof(ch));
    if (ch == EOF)
        throw Exception("Reached end of template");
    return ch;
}

int
Template::Lexer::peek_char()
{
    auto ch = this->sstr.peek();
    return ch;
}

// template

Template::Template(const std::string & templ) : lexer(templ) {}

std::string
Template::subst(const Params & params)
{
    std::string s;
    Token token = this->lexer.peek();
    while (token.type != Token::End) {
        token = this->lexer.read();
        if (token.type == Token::OpenVar) {
            this->lexer.set_mode(Lexer::Var);
            auto var_token = this->lexer.read();
            auto next = this->lexer.peek();
            if (var_token.type == Token::Variable && next.type == Token::CloseVar) {
                this->lexer.read();
                s += params.get(var_token.val);
                this->lexer.set_mode(Lexer::Text);
            }
            else
                throw Exception("Malformed template");
        }
        else {
            s += token.val;
        }
        token = this->lexer.peek();
    }
    return s;
}

std::string
Template::build(const std::string & tpl, const Params & params)
{
    Template t(tpl);
    auto str = t.subst(params);
    return str;
}

} // namespace openclcpp_lite
