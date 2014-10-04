import Control.Monad
import Text.ParserCombinators.Parsec
import Text.ParserCombinators.Parsec.Expr
import Text.ParserCombinators.Parsec.Language
import qualified Text.ParserCombinators.Parsec.Token as Token

data Expression = AddExpression Expression Term
                | SubExpression Expression Term
                | TermExpression Term

data Term = MulTerm Term Factor
          | DivTerm Term Factor
          | FactorTerm Factor

data Factor = ParFactor Expression
            | LiteralFactor String
            | NumberFactor Integer

instance Show Expression where
  show (AddExpression e t) = show e ++ "+" ++ show t
  show (SubExpression e t) = show e ++ "-" ++ show t
  show (TermExpression t) = show t

instance Show Term where
  show (MulTerm t f) = show t ++ "*" ++ show f
  show (DivTerm t f) = show t ++ "/" ++ show f
  show (FactorTerm f) = show f

instance Show Factor where
  show (ParFactor e) = "(" ++ show e ++ ")"
  show (LiteralFactor s) = s
  show (NumberFactor n) = show n

p = show $ AddExpression (TermExpression (FactorTerm (LiteralFactor "x"))) (FactorTerm (NumberFactor 5))

-- Expression parser

parseAddExpression :: Parser Expression
parseAddExpression = do
  e <- parseExpression
  char '+'
  t <- parseTerm
  return $ AddExpression e t

parseSubExpression :: Parser Expression
parseSubExpression = do
  e <- parseExpression
  char '-'
  t <- parseTerm
  return $ SubExpression e t

parseTermExpression :: Parser Expression
parseTermExpression = do
  t <- parseTerm
  return $ TermExpression t

parseExpression :: Parser Expression
parseExpression = parseAddExpression
              <|> parseSubExpression
              <|> parseTermExpression

-- Term parser

parseMulTerm :: Parser Term
parseMulTerm = do
  t <- parseTerm
  char '*'
  f <- parseFactor
  return $ MulTerm t f

parseDivTerm :: Parser Term
parseDivTerm = do
  t <- parseTerm
  char '/'
  f <- parseFactor
  return $ DivTerm t f

parseFactorTerm :: Parser Term
parseFactorTerm = do
  f <- parseFactor
  return $ FactorTerm f

parseTerm :: Parser Term
parseTerm = parseMulTerm
        <|> parseDivTerm
        <|> parseFactorTerm

-- Factor parser

parseParFactor :: Parser Factor
parseParFactor = do
  char '('
  e <- parseExpression
  char ')'
  return $ ParFactor e

parseLiteralFactor :: Parser Factor
parseLiteralFactor = do
  x <- many1 letter
  return $ LiteralFactor x

parseNumberFactor :: Parser Factor
parseNumberFactor = liftM (NumberFactor . read) $ many1 digit

parseFactor :: Parser Factor
parseFactor = parseParFactor
          <|> parseLiteralFactor
          <|> parseNumberFactor

-- Main

readExpr :: String -> String
readExpr input = case parse parseTerm "diff" input of
  Left err -> "No match: " ++ show err
  Right val -> "Found value: " ++ show val

main :: IO()
main = do
  line <- getLine
  putStrLn (readExpr line)
