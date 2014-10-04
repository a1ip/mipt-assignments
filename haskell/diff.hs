import Control.Monad
import Text.ParserCombinators.Parsec
import Text.ParserCombinators.Parsec.Expr
import Text.ParserCombinators.Parsec.Language
import qualified Text.ParserCombinators.Parsec.Token as Token

-- Datatypes

data UnOp = Minus
            deriving(Eq)
data BinOp = Add
           | Sub
           | Mul
           | Div
           deriving(Eq)
data Func = Sin
          | Cos
          | Exp
          | Log
          deriving(Eq)
data Expr = Number Integer
          | Ident String
          | UnOp UnOp Expr
          | BinOp BinOp Expr Expr
          | Call Func Expr
          deriving(Eq)

instance Show UnOp where
  show Minus = "-"

instance Show BinOp where
  show Add = "+"
  show Sub = "-"
  show Mul = "*"
  show Div = "/"

instance Show Func where
  show Sin = "sin"
  show Cos = "cos"
  show Exp = "exp"
  show Log = "log"

instance Show Expr where
  show (Number n) = show n
  show (Ident i) = i

  show (UnOp o (Number n)) = show o ++ show n
  show (UnOp o (Ident i)) = show o ++ i

  show (UnOp o e) = show o ++ "(" ++ show e ++ ")"

  show (BinOp o (Number n) (Number m)) = show n ++ show o ++ show m
  show (BinOp o (Number n) (Ident i)) = show n ++ show o ++ i
  show (BinOp o (Ident i) (Number n)) = i ++ show o ++ show n
  show (BinOp o (Ident i) (Ident k)) = i ++ show o ++ k

  show (BinOp Mul e1 (BinOp Mul e2 e3)) = show e1 ++ "*" ++ show e2 ++ "*" ++ show e3
  show (BinOp Mul (BinOp Mul e1 e2) e3) = show e1 ++ "*" ++ show e2 ++ "*" ++ show e3

  show (BinOp o e1 e2) = "(" ++ show e1 ++ ")" ++ show o ++ "(" ++ show e2 ++ ")"

  show (Call f e) = show f ++ "(" ++ show e ++ ")"

-- Diff

diff :: Expr -> String -> Expr

diff (Number n) x = Number 0

diff (Ident i) x
  | (i == x) = Number 1
  | otherwise = Number 0

diff (UnOp o e) x = UnOp o (diff e x)

diff (BinOp Mul e1 e2) x =
  BinOp Add
    (BinOp Mul (diff e1 x) e2)
    (BinOp Mul e1 (diff e2 x))

diff (BinOp Div e1 e2) x =
  BinOp Div
    (BinOp Sub
      (BinOp Mul (diff e1 x) e2)
      (BinOp Mul e1 (diff e2 x)))
    (BinOp Mul e2 e2)

diff (BinOp o e1 e2) x = BinOp o (diff e1 x) (diff e2 x)

diff (Call Sin e) x = BinOp Mul (Call Cos e) (diff e x)
diff (Call Cos e) x = UnOp Minus (BinOp Mul (Call Sin e) (diff e x))
diff (Call Exp e) x = BinOp Mul (Call Exp e) (diff e x)
diff (Call Log e) x = BinOp Div (diff e x) e

-- Simplify

simplify :: Expr -> Expr

simplify (BinOp Add (Number n1) (Number n2)) = Number (n1 + n2)
simplify (BinOp Sub (Number n1) (Number n2)) = Number (n1 - n2)
simplify (BinOp Mul (Number n1) (Number n2)) = Number (n1 * n2)

simplify (UnOp Minus (Number n)) = Number (-n)

simplify (BinOp Add e (Number 0)) = e
simplify (BinOp Add (Number 0) e) = e

simplify (BinOp Mul e (Number 0)) = Number 0
simplify (BinOp Mul (Number 0) e) = Number 0

simplify (BinOp Sub e (Number 0)) = e
simplify (BinOp Sub (Number 0) e) = UnOp Minus e

simplify (BinOp Div (Number 0) e) = Number 0

simplify (BinOp Mul e (Number 1)) = e
simplify (BinOp Mul (Number 1) e) = e

simplify (BinOp Sub e1 e2) | (e1 == e2) = Number 0
simplify (BinOp Div e1 e2) | (e1 == e2) = Number 1

simplify (BinOp Add e1 e2) | (e1 == e2) = BinOp Mul (Number 2) e1
simplify (BinOp Add (BinOp Mul (Number n1) e1) e2)
  | (e1 == e2) = BinOp Mul (Number (n1 + 1)) e1
simplify (BinOp Add e1 (BinOp Mul (Number n2) e2))
  | (e1 == e2) = BinOp Mul (Number (n2 + 1)) e1
simplify (BinOp Add (BinOp Mul (Number n1) e1) (BinOp Mul (Number n2) e2))
  | (e1 == e2) = BinOp Mul (Number (n1 + n2)) e1

simplify (BinOp Mul (Number n1) (BinOp Mul e (Number n2)))
  = BinOp Mul (Number (n1 * n2)) e
simplify (BinOp Mul (Number n1) (BinOp Mul (Number n2) e))
  = BinOp Mul (Number (n1 * n2)) e
simplify (BinOp Mul (BinOp Mul e (Number n1)) (Number n2))
  = BinOp Mul (Number (n1 * n2)) e
simplify (BinOp Mul (BinOp Mul (Number n1) e) (Number n2))
  = BinOp Mul (Number (n1 * n2)) e

simplify (BinOp Mul (Number n1) (BinOp Div (Number n2) e))
  = BinOp Div (Number (n1 * n2)) e
simplify (BinOp Mul (BinOp Div (Number n1) e) (Number n2))
  = BinOp Div (Number (n1 * n2)) e

simplify (BinOp Div (Number n1) (BinOp Div e (Number n2)))
  = BinOp Div (Number (n1 * n2)) e

simplify (BinOp Mul e (Number n)) = BinOp Mul (Number n) e

simplify (BinOp Mul (BinOp Mul (Number n) e1) e2)
  = BinOp Mul (Number n) (BinOp Mul e1 e2)
simplify (BinOp Mul e1 (BinOp Mul (Number n) e2))
  = BinOp Mul (Number n) (BinOp Mul e1 e2)

simplify (UnOp o e) = UnOp o (simplify e)
simplify (BinOp o e1 e2) = BinOp o (simplify e1) (simplify e2)
simplify (Call f e) = Call f (simplify e)

simplify e = e

fullSimplify :: Expr -> Expr
fullSimplify e
  | (se == e) = e
  | otherwise = fullSimplify se
  where se = simplify e

-- Parser

langDef = emptyDef {
  Token.identStart = letter,
  Token.identLetter = letter,
  Token.reservedOpNames = ["+", "-", "*", "/"]
}

lexer = Token.makeTokenParser langDef

identifier = Token.identifier lexer
integer    = Token.integer    lexer
parens     = Token.parens     lexer
reservedOp = Token.reservedOp lexer

operators = [
  [Prefix (reservedOp "-" >> return (UnOp Minus))],
  [Infix  (reservedOp "*" >> return (BinOp Mul )) AssocLeft,
   Infix  (reservedOp "/" >> return (BinOp Div )) AssocLeft],
  [Infix  (reservedOp "+" >> return (BinOp Add )) AssocLeft,
   Infix  (reservedOp "-" >> return (BinOp Sub )) AssocLeft]]

expression :: Parser Expr
expression = buildExpressionParser operators term

callParser :: String -> Func -> Parser Expr
callParser str func = do
  name <- string str
  expr <- parens $ expression
  return $ Call func expr

call :: Parser Expr
call = callParser "sin" Sin
   <|> callParser "cos" Cos
   <|> callParser "exp" Exp
   <|> callParser "log" Log

term = parens expression
   <|> try call
   <|> liftM Ident identifier
   <|> liftM Number integer

parseString :: String -> Expr
parseString str =
  case parse expression "diff" str of
    Left err -> error $ show err
    Right val -> val

-- Main

main :: IO()
main = do
  line <- getLine
  putStrLn $ show $ fullSimplify $ diff (parseString line) "x"
