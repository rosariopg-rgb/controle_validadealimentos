-- 4.1 CRIAÇÃO DAS TABELAS

CREATE TABLE categoria (
    id   INT PRIMARY KEY,
    nome VARCHAR(30) NOT NULL UNIQUE
);

CREATE TABLE local_armazenamento (
    id   INT PRIMARY KEY,
    nome VARCHAR(30) NOT NULL UNIQUE
);

CREATE TABLE situacao (
    id        INT PRIMARY KEY,
    descricao VARCHAR(30) NOT NULL UNIQUE
);

CREATE TABLE alimento (
    id           INT PRIMARY KEY,
    nome         VARCHAR(30) NOT NULL,
    id_categoria INT NOT NULL,
    id_local     INT NOT NULL,
    FOREIGN KEY (id_categoria) REFERENCES categoria (id),
    FOREIGN KEY (id_local) REFERENCES local_armazenamento (id)
);

CREATE TABLE lote (
    id            INT PRIMARY KEY,
    id_alimento   INT NOT NULL,
    id_situacao   INT NOT NULL,
    quantidade    INT NOT NULL CHECK (quantidade >= 0),
    data_validade DATE NOT NULL,
    FOREIGN KEY (id_alimento) REFERENCES alimento (id),
    FOREIGN KEY (id_situacao) REFERENCES situacao (id)
);


-- 4.2 INSERÇÃO DE DADOS

-- Tabelas de apoio
INSERT INTO categoria (id, nome) VALUES
    (1, 'Laticinios'),
    (2, 'Carnes'),
    (3, 'Verduras'),
    (4, 'Graos e Secos');

INSERT INTO local_armazenamento (id, nome) VALUES
    (1, 'Geladeira'),
    (2, 'Freezer'),
    (3, 'Despensa');

INSERT INTO situacao (id, descricao) VALUES
    (1, 'VENCIDO'),
    (2, 'VENCE HOJE'),
    (3, 'EM DIA');


-- Alimentos
INSERT INTO alimento (id, nome, id_categoria, id_local) VALUES
    (1,  'Leite',       1, 1),
    (2,  'Queijo',      1, 1),
    (3,  'Iogurte',     1, 1),
    (4,  'Frango',      2, 2),
    (5,  'Carne_Moida', 2, 2),
    (6,  'Alface',     3, 1),
    (7,  'Tomate',     3, 1),
    (8,  'Arroz',      4, 3),
    (9,  'Feijao',     4, 3),
    (10, 'Farinha',    4, 3);


-- Lotes
-- Situação calculada para a data de referência 2026-11-19

INSERT INTO lote (id, id_alimento, id_situacao, quantidade, data_validade) VALUES
    (1,  1,  1, 12, '2026-11-15'),
    (2,  1,  3, 24, '2026-11-30'),
    (3,  2,  2,  5, '2026-11-19'),
    (4,  3,  1, 10, '2026-11-17'),
    (5,  4,  3, 20, '2026-12-20'),
    (6,  5,  2,  8, '2026-11-19'),
    (7,  6,  1,  6, '2026-11-18'),
    (8,  7,  3, 15, '2026-11-25'),
    (9,  8,  3, 30, '2027-05-10'),
    (10, 9,  3, 25, '2027-03-15'),
    (11, 10, 1, 10, '2026-11-10'),
    (12, 4,  3, 15, '2026-11-28');


-- 4.3 RELATÓRIOS

-- RELATÓRIO 1: Listagem geral dos alimentos

SELECT a.nome AS alimento,
       c.nome AS categoria,
       l.nome AS local,
       lt.quantidade,
       lt.data_validade,
       s.descricao AS situacao
FROM lote lt
JOIN alimento a ON a.id = lt.id_alimento
JOIN categoria c ON c.id = a.id_categoria
JOIN local_armazenamento l ON l.id = a.id_local
JOIN situacao s ON s.id = lt.id_situacao
ORDER BY lt.data_validade;


-- RELATÓRIO 2: Alertas de validade

SELECT a.nome AS alimento,
       lt.quantidade,
       lt.data_validade,
       s.descricao AS situacao
FROM lote lt
JOIN alimento a ON a.id = lt.id_alimento
JOIN situacao s ON s.id = lt.id_situacao
WHERE lt.id_situacao IN (1, 2)
ORDER BY lt.id_situacao, lt.data_validade;


-- RELATÓRIO 3: Alimentos e quantidade total por categoria

SELECT c.nome AS categoria,
       COUNT(DISTINCT a.id) AS qtd_alimentos,
       COALESCE(SUM(lt.quantidade), 0) AS total_unidades
FROM categoria c
LEFT JOIN alimento a ON a.id_categoria = c.id
LEFT JOIN lote lt ON lt.id_alimento = a.id
GROUP BY c.nome
ORDER BY c.nome;


-- RELATÓRIO 4: Quantidade total por local de armazenamento

SELECT l.nome AS local,
       COUNT(DISTINCT a.id) AS qtd_alimentos,
       COALESCE(SUM(lt.quantidade), 0) AS total_unidades
FROM local_armazenamento l
LEFT JOIN alimento a ON a.id_local = l.id
LEFT JOIN lote lt ON lt.id_alimento = a.id
GROUP BY l.nome
ORDER BY l.nome;


-- RELATÓRIO 5: Resumo de lotes por situação

SELECT s.descricao AS situacao,
       COUNT(lt.id) AS qtd_lotes
FROM situacao s
LEFT JOIN lote lt ON lt.id_situacao = s.id
GROUP BY s.id, s.descricao
ORDER BY s.id;


-- RELATÓRIO 6: Situação recalculada pela data

SELECT a.nome AS alimento,
       lt.data_validade,
       CASE
           WHEN lt.data_validade < '2026-11-19' THEN 'VENCIDO'
           WHEN lt.data_validade = '2026-11-19' THEN 'VENCE HOJE'
           ELSE 'EM DIA'
       END AS situacao_calculada
FROM lote lt
JOIN alimento a ON a.id = lt.id_alimento
ORDER BY lt.data_validade;


-- RELATÓRIO 7: Lotes que vencem nos próximos 7 dias

SELECT a.nome AS alimento,
       lt.quantidade,
       lt.data_validade
FROM lote lt
JOIN alimento a ON a.id = lt.id_alimento
WHERE lt.data_validade BETWEEN '2026-11-19' AND '2026-11-26'
ORDER BY lt.data_validade;


-- RELATÓRIO 8: Alimentos com mais de um lote

SELECT a.nome AS alimento,
       COUNT(lt.id) AS qtd_lotes,
       SUM(lt.quantidade) AS total_unidades
FROM alimento a
JOIN lote lt ON lt.id_alimento = a.id
GROUP BY a.nome
HAVING COUNT(lt.id) > 1;
